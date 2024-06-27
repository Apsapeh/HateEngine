use serde::{Deserialize, Serialize};

#[derive(Deserialize, Serialize)]
pub struct HpprjToml {
    pub password: String,
    pub pack_path: Option<String>,
    pub resources: Vec<HpprjResource>,
}

#[derive(Deserialize, Serialize)]
pub struct HpprjResource {
    pub path: String,
    pub name: String,
    pub is_relative: bool,
}
