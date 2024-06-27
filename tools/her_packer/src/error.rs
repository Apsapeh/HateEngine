pub enum Error {
    PasswordIsNotASCII,
    PasswordIsNotValidLength,
}

impl std::fmt::Debug for Error {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Error::PasswordIsNotASCII => f.write_str("Password is not ASCII"),
            Error::PasswordIsNotValidLength => f.write_str("Password is not valid length"),
        }
    }
}
