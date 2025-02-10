use std::os::{
    fd,
    raw::{c_char, c_int, c_void},
};

extern "C" {
    pub fn asm_strlen(string: *const c_char) -> usize;
    pub fn asm_write(fd: fd::RawFd, buf: *const c_void, count: isize) -> isize;
    pub fn asm_read(fd: fd::RawFd, buf: *const c_void, count: isize) -> isize;
    pub fn asm_strcmp(s1: *const c_char, s2: *const c_char) -> c_int;
    pub fn asm_strcpy(dest: *mut c_char, src: *const c_char) -> *mut c_char;
    pub fn asm_strdup(s: *const c_char) -> *mut c_void;
}

mod safe_asm {
    use super::*;
    use std::{error::Error, ffi::CString, fmt::Display, str::FromStr};

    #[allow(unused)]
    #[derive(Debug, PartialEq)]
    pub enum LibasmErrorKind {
        Errno(isize),
        GenericError(String),
    }

    impl Display for LibasmErrorKind {
        fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            match self {
                Self::Errno(val) => write!(f, "failed with errno set to {val}"),
                Self::GenericError(reason) => write!(f, "{}", reason),
            }
        }
    }
    impl Error for LibasmErrorKind {}

    #[allow(unused)]
    #[derive(Debug, PartialEq)]
    pub enum CmpResult {
        Equal,
        NonEqual,
    }

    #[allow(unused)]
    pub fn safe_strlen(string: &str) -> Result<usize, LibasmErrorKind> {
        let len: usize;
        unsafe {
            let ptr = CString::from_str(string)
                .map_err(|val| LibasmErrorKind::GenericError(val.to_string()))?;
            if ptr.is_empty() {
                Err(LibasmErrorKind::GenericError(
                    "the string would segfault".to_string(),
                ))?;
            }
            len = asm_strlen(ptr.as_ptr());
        }
        Ok(len)
    }
    //pub fn safe_write(fd: fd::RawFd, buf: *const c_void, count: isize) -> isize;
    //pub fn safe_read(fd: fd::RawFd, buf: *const c_void, count: isize) -> isize;
    #[allow(unused)]
    pub fn safe_strcmp(s1: &str, s2: &str) -> CmpResult {
        if s1.is_empty() && s2.is_empty() {
            return CmpResult::Equal;
        } else if (s1.is_empty() && !s2.is_empty()) || (!s1.is_empty() && s2.is_empty()) {
            return CmpResult::NonEqual;
        }
        let c_s1 = CString::from_str(s1).unwrap();
        let c_s2 = CString::from_str(s2).unwrap();

        let result: i32;
        unsafe {
            result = asm_strcmp(c_s1.as_ptr(), c_s2.as_ptr());
        }

        if result != 0 {
            return CmpResult::NonEqual;
        }
        CmpResult::Equal
    }
    //pub fn safe_strcpy(dest: *mut c_char, src: *const c_char) -> *const c_char;
    #[allow(unused)]
    pub fn safe_strdup(s: &str) -> Result<String, LibasmErrorKind> {
        let len = match safe_strlen(s) {
            Ok(val) => val,
            Err(err) => Err(LibasmErrorKind::GenericError(err.to_string()))?,
        };
        let result_string: String;

        let c_string = CString::from_str(s).unwrap();
        unsafe {
            let raw_memory = asm_strdup(c_string.as_ptr());
            result_string = String::from_raw_parts(
                std::mem::transmute::<*mut c_void, *mut u8>(raw_memory),
                len,
                len + 1,
            );
        }
        Ok(result_string)
    }
}

#[cfg(test)]
mod strlen_tests {
    use super::*;

    #[test]
    fn string_cinco() {
        assert_eq!(safe_asm::safe_strlen("cinco"), Ok(5));
    }

    #[test]
    fn would_segfault() {
        assert!(safe_asm::safe_strlen("").is_err());
    }

    #[test]
    fn big_string() {
        assert_eq!(safe_asm::safe_strlen("cincocincocincocinco"), Ok(20));
    }
}

#[cfg(test)]
mod strcmp_tests {
    use safe_asm::CmpResult;

    use super::*;

    #[test]
    fn equal_strings() {
        assert_eq!(safe_asm::safe_strcmp("cinco", "cinco"), CmpResult::Equal);
    }

    #[test]
    fn different_string() {
        assert_eq!(safe_asm::safe_strcmp("cinca", "cinco"), CmpResult::NonEqual);
    }

    #[test]
    fn empty_string() {
        assert_eq!(safe_asm::safe_strcmp("", "cinco"), CmpResult::NonEqual);
    }
}

#[cfg(test)]
mod strdup_tests {
    use super::*;

    #[test]
    fn empty_string() {
        assert!(safe_asm::safe_strdup("").is_err());
    }

    #[test]
    fn simple_string_slice() {
        assert_eq!(safe_asm::safe_strdup("cinca").unwrap(), "cinca");
    }

    #[test]
    fn simple_string() {
        let example_string = "Essa aqui".to_string();

        assert_eq!(
            safe_asm::safe_strdup(&example_string).unwrap(),
            example_string
        );
    }

    #[test]
    fn long_string() {
        let example_string = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAaaaaaaaaaaakmnaskdljfgnklasfjdhgklasdjfhgklsjdfhgkl;sdjfhg".to_string();

        assert_eq!(
            safe_asm::safe_strdup(&example_string).unwrap(),
            example_string
        );
    }
}
