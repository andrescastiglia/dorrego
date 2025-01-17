use super::*;
use yew::prelude::*;
use yew_router::prelude::*;

#[derive(Routable, Clone, PartialEq)]
pub enum Router {
    #[at("/")]
    Home,
    #[at("/news")]
    News,
    #[at("/project")]
    Project,
    #[at("/report")]
    Report,
    #[not_found]
    #[at("/404")]
    NotFound,
}

pub fn switch(routes: Router) -> Html {
    match routes {
        Router::Home | Router::News => html! { <TagNews /> },
        Router::Project => html! { <TagProject /> },
        Router::Report => html! { <TagReport /> },
        Router::NotFound => html! { <TagNotFound /> },
    }
}

#[function_component]
pub fn TagBody() -> Html {
    html! {
        <BrowserRouter>
            <TagNav />
            <div class="flex-grow pt-16">
                <div class="max-w-7xl mx-auto px-4 py-6">
                    <Switch<Router> render={ switch } />
                </div>
            </div>
        </BrowserRouter>
    }
}
