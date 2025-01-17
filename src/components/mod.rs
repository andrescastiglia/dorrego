pub mod body;
pub mod footer;
pub mod nav;
pub mod news;
pub mod notfound;
pub mod project;
pub mod report;

pub use body::TagBody;
pub use footer::TagFooter;
pub use nav::TagNav;
pub use news::TagNews;
pub use notfound::TagNotFound;
pub use project::TagProject;
pub use report::TagReport;

pub use body::Router;