#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")] // hide console window on Windows in release

use std::path::PathBuf;

use eframe::egui::{self, FontFamily, FontId, FontSelection, RichText};
use hpprj_toml::{HpprjResource, HpprjToml};
use rfd::MessageDialogResult;
use translit::ToLatin;

mod error;
mod her_struct;
mod hpprj_toml;

use her_struct::*;

fn main() -> Result<(), eframe::Error> {
    let args: Vec<String> = std::env::args().collect();

    let mut app = MyApp::default();
    if args.len() > 1 {
        let path = PathBuf::from(&args[1]);
        app.project_path = Some(path);
        app.parse_proj_file();
    }

    let options = eframe::NativeOptions {
        viewport: egui::ViewportBuilder::default()
            .with_inner_size([1280.0, 720.0]) // wide enough for the drag-drop overlay text
            .with_drag_and_drop(true)
            .with_resizable(false),
        default_theme: eframe::Theme::Light,
        follow_system_theme: false,
        ..Default::default()
    };

    eframe::run_native(
        "HER Packer",
        options,
        Box::new(|_cc| Box::new(app)),
    )
}

#[derive(PartialEq)]
struct ResourceFile {
    path: PathBuf,
    is_relative: bool,
    size: String,
    name: String,
}

#[derive(Default)]
struct MyApp {
    password: String,
    resource_files: Vec<ResourceFile>,
    project_path: Option<PathBuf>,
    packed_path: Option<PathBuf>,
}

impl eframe::App for MyApp {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        egui::CentralPanel::default().show(ctx, |ui| {
            ui.horizontal(|ui| {
                if ui.button("Open project").clicked() {
                    self.on_btn_open_project()
                }

                if ui.button("Save project").clicked() {
                    /*if let Some(path) = rfd::FileDialog::new().pick_file() {
                        self.picked_path = Some(path.display().to_string());
                    }*/
                    self.on_btn_save_project();
                }

                if ui.button("Save as...").clicked() {
                    self.on_btn_save_as();
                }

                if ui.button("Import resource").clicked() {
                    /*if let Some(path) = rfd::FileDialog::new().pick_file() {
                        self.picked_path = Some(path.display().to_string());
                    }*/
                    self.on_btn_import_resource();
                }
                //});

                ui.with_layout(
                    egui::Layout::default().with_cross_align(egui::Align::RIGHT),
                    |ui| {
                        ui.horizontal(|ui| {
                            if ui.button("Pack as...").clicked() {
                                self.on_btn_pack_as();
                            }
                            if ui.button("Pack").clicked() {
                                self.on_btn_pack();
                            }

                            ui.text_edit_singleline(&mut self.password);
                            ui.label("Password:");
                            self.password = fix_name_ascii(&self.password);
                        });
                    },
                );
            });

            // Show dropped files (if any):
            if !self.resource_files.is_empty() {
                ui.group(|ui| {
                    egui::ScrollArea::vertical().show(ui, |ui| {
                        ui.label("Resources:");

                        let mut i = 0u32;
                        let mut remove_vec = Vec::new();
                        for file in &mut self.resource_files {
                            i += 1;
                            let mut path = format!("{}", file.path.display().to_string());

                            ui.horizontal(|ui| {
                                ui.label(RichText::new(format!("{i}.")).weak().size(20.0));
                                ui.label(RichText::new("Path:").strong().size(20.0));
                                ui.add(
                                    egui::TextEdit::singleline(&mut path)
                                        .font(FontSelection::FontId(FontId::new(
                                            20.,
                                            FontFamily::Proportional,
                                        )))
                                        .desired_width(430.0)
                                        .hint_text("Write something here"),
                                );
                                ui.add_space(10.0);
                                ui.label(RichText::new("Name:").strong().size(20.0));
                                ui.add(
                                    egui::TextEdit::singleline(&mut file.name)
                                        .font(FontSelection::FontId(FontId::new(
                                            20.,
                                            FontFamily::Proportional,
                                        )))
                                        .desired_width(330.0)
                                        .hint_text("Resource name"),
                                );
                                file.name = fix_name_ascii(&file.name);

                                ui.add_space(10.0);
                                ui.label(RichText::new(&file.size).weak().size(20.0));

                                ui.with_layout(
                                    egui::Layout::default().with_cross_align(egui::Align::RIGHT),
                                    |ui| {
                                        ui.horizontal(|ui| {
                                            if ui
                                                .button(RichText::new("Remove").size(20.0))
                                                .clicked()
                                            {
                                                remove_vec.push(file.name.clone());
                                            }

                                            //let mut is_relative = false;
                                            ui.checkbox(&mut file.is_relative, "Rel.");
                                        });
                                    },
                                );
                            });
                        }

                        for name in remove_vec {
                            self.resource_files.retain(|f| f.name != name);
                        }
                    });
                });
            }
        });

        /*egui::TopBottomPanel::bottom("bottom_panel").show(ctx, |ui| {
            ui.horizontal(|ui| {
                ui.label(RichText::new("Status label"));
            });
        });*/

        preview_files_being_dropped(ctx);

        // Collect dropped files:
        //let mut
        ctx.input(|i| {
            /*if !i.raw.dropped_files.is_empty() {
                self.dropped_files.clone_from(&i.raw.dropped_files);
            }*/

            for file in &i.raw.dropped_files {
                let path = match file.path.clone() {
                    Some(path) => path,
                    None => continue,
                };

                let size_bytes = std::fs::metadata(&path).unwrap().len();
                let size = if size_bytes < 1024 {
                    format!("{} B", size_bytes)
                } else if size_bytes < 1024 * 1024 {
                    format!("{:.2} KiB", size_bytes as f64 / 1024.0)
                } else if size_bytes < 1024 * 1024 * 1024 {
                    format!("{:.2} MiB", size_bytes as f64 / 1024.0 / 1024.0)
                } else {
                    format!("{:.2} GiB", size_bytes as f64 / 1024.0 / 1024.0 / 1024.0)
                };
                let mut name = fix_name_ascii(path.file_name().unwrap().to_str().unwrap());

                // find name
                while self.resource_files.iter().any(|f| f.name == name) {
                    name = format!("_{}", name);
                }

                self.resource_files.push(ResourceFile {
                    path,
                    size,
                    name,
                    is_relative: false,
                });
            }
        });
    }
}

impl MyApp {
    fn on_btn_open_project(&mut self) {
        let path = rfd::FileDialog::new()
            .add_filter("", &["hpprj"])
            .pick_file();

        let path = match path {
            Some(p) => p,
            None => {
                rfd_msg_dialog(
                    "Open project error",
                    "Я хз чо это за ошибка",
                    rfd::MessageLevel::Error,
                );
                return;
            }
        };

        self.project_path = Some(path);
        self.parse_proj_file();
    }

    fn on_btn_save_project(&mut self) {
        match &self.project_path {
            Some(_) => self.save_proj_file(),
            None => self.on_btn_save_as(),
        }
    }

    fn on_btn_save_as(&mut self) {
        let path = rfd::FileDialog::new()
            .set_file_name("untitled.hpprj")
            .add_filter("", &["hpprj"])
            .save_file();

        let path = match path {
            Some(p) => p,
            None => {
                rfd_msg_dialog(
                    "Save project error",
                    "Я хз чо это за ошибка",
                    rfd::MessageLevel::Error,
                );
                return;
            }
        };

        self.project_path = Some(path);
        self.save_proj_file();
    }

    fn on_btn_import_resource(&mut self) {
        rfd_msg_dialog(
            "Import resource",
            "Not implemented yet",
            rfd::MessageLevel::Info,
        );
    }

    fn on_btn_pack(&mut self) {
        match &self.packed_path {
            Some(_) => self.pack_proj(),
            None => self.on_btn_pack_as(),
        }
    }

    fn on_btn_pack_as(&mut self) {
        let path = rfd::FileDialog::new()
            .set_file_name("untitled")
            .add_filter("", &["her"])
            .save_file();

        let path = match path {
            Some(p) => p,
            None => {
                rfd_msg_dialog(
                    "Save project error",
                    "Я хз чо это за ошибка",
                    rfd::MessageLevel::Error,
                );
                return;
            }
        };

        self.packed_path = Some(path);
        self.pack_proj();
    }

    fn parse_proj_file(&mut self) {
        let proj_file = std::fs::read_to_string(&self.project_path.clone().unwrap()).unwrap();
        let proj: hpprj_toml::HpprjToml = toml::from_str(&proj_file).unwrap();

        self.password = proj.password;

        if let Some(p) = proj.pack_path {
            let path = self.project_path.clone().unwrap().parent().unwrap().join(p);
            self.packed_path = Some(path);
        }

        self.resource_files.clear();
        for res in proj.resources {
            let path = self
                .project_path
                .clone()
                .unwrap()
                .parent()
                .unwrap()
                .join(&res.path);

            if !path.exists() {
                eprintln!("File not found: {}", path.display().to_string());
                continue;
            }

            let size_bytes = std::fs::metadata(&path).unwrap().len();
            let size = if size_bytes < 1024 {
                format!("{} B", size_bytes)
            } else if size_bytes < 1024 * 1024 {
                format!("{:.2} KiB", size_bytes as f64 / 1024.0)
            } else if size_bytes < 1024 * 1024 * 1024 {
                format!("{:.2} MiB", size_bytes as f64 / 1024.0 / 1024.0)
            } else {
                format!("{:.2} GiB", size_bytes as f64 / 1024.0 / 1024.0 / 1024.0)
            };

            self.resource_files.push(ResourceFile {
                path,
                size,
                name: res.name,
                is_relative: res.is_relative,
            });
        }
    }

    fn save_proj_file(&self) {
        let mut resources = Vec::new();
        for file in &self.resource_files {
            let path = if file.is_relative {
                let hpprj_path = self.project_path.clone().unwrap();
                let hpprj_path = hpprj_path.parent().unwrap();

                pathdiff::diff_paths(&file.path, &hpprj_path).unwrap()
            } else {
                file.path.clone()
            };

            resources.push(HpprjResource {
                path: path.display().to_string(),
                name: file.name.clone(),
                is_relative: file.is_relative,
            })
        }

        let hpprj_path = self.project_path.clone().unwrap();
        let hpprj_path = hpprj_path.parent().unwrap();
        let pack_path = match self.packed_path.clone() {
            Some(p) => Some(
                pathdiff::diff_paths(&p, &hpprj_path)
                    .unwrap()
                    .display()
                    .to_string(),
            ),
            None => None,
        };

        let toml_proj = HpprjToml {
            password: self.password.clone(),
            pack_path,
            resources,
        };

        let toml = toml::to_string(&toml_proj).unwrap();
        std::fs::write(&self.project_path.clone().unwrap(), toml).unwrap();
    }

    fn pack_proj(&self) {
        if rfd::MessageDialog::new()
            .set_level(rfd::MessageLevel::Info)
            .set_title("Pack?")
            .set_buttons(rfd::MessageButtons::YesNo)
            .set_description(format!(
                "Password: {}\nPath: {}",
                self.password,
                self.packed_path.clone().unwrap().display().to_string()
            ))
            .show()
            == MessageDialogResult::No
        {
            return;
        }

        if (self.password.len() < 5 || self.password.len() > 56) && self.password.len() % 8 != 0 {
            rfd_msg_dialog(
                "Password error",
                "Password must be between 5 and 56 characters, and len must be multiply 8 bits",
                rfd::MessageLevel::Error,
            );
            return;
        }

        let mut her = HerFile::new(&self.password).unwrap();

        for file in &self.resource_files {
            let resource = HerResource::new(&file.name, &file.path.display().to_string());
            her.add_resource(resource);
        }

        her.save(&self.packed_path.clone().unwrap().display().to_string());
        rfd_msg_dialog(
            "Success",
            "Project packed successfully",
            rfd::MessageLevel::Info,
        );
    }
}

/// Preview hovering files:
fn preview_files_being_dropped(ctx: &egui::Context) {
    use egui::*;
    use std::fmt::Write as _;

    if !ctx.input(|i| i.raw.hovered_files.is_empty()) {
        let text = ctx.input(|i| {
            let mut text = "Dropping files:\n".to_owned();
            for file in &i.raw.hovered_files {
                if let Some(path) = &file.path {
                    write!(text, "\n{}", path.display()).ok();
                } else if !file.mime.is_empty() {
                    write!(text, "\n{}", file.mime).ok();
                } else {
                    text += "\n???";
                }
            }
            text
        });

        let painter =
            ctx.layer_painter(LayerId::new(Order::Foreground, Id::new("file_drop_target")));

        let screen_rect = ctx.screen_rect();
        painter.rect_filled(screen_rect, 0.0, Color32::from_black_alpha(192));
        painter.text(
            screen_rect.center(),
            Align2::CENTER_CENTER,
            text,
            TextStyle::Heading.resolve(&ctx.style()),
            Color32::WHITE,
        );
    }
}

fn fix_name_ascii(name: &str) -> String {
    let trasliterator = translit::Passport2013::new();
    let res = trasliterator.to_latin(name);
    res.chars()
        .filter(|c| c.is_ascii_alphanumeric() || c.is_ascii_punctuation())
        .collect()
}

// RFD fns

fn rfd_msg_dialog(title: &str, msg: &str, level: rfd::MessageLevel) {
    rfd::MessageDialog::new()
        .set_level(level)
        .set_title(title)
        .set_description(msg)
        .set_buttons(rfd::MessageButtons::Ok)
        .show();
}
