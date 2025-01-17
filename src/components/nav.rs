use super::Router;
use yew::prelude::*;
use yew_router::prelude::*;

#[function_component]
pub fn TagNav() -> Html {
    let is_hidden = use_state(|| true);
    let toggle_menu = {
        let is_hidden = is_hidden.clone();
        Callback::from(move |event: MouseEvent| {
            event.prevent_default();
            is_hidden.set(!*is_hidden);
        })
    };

    html! {
        <nav class="bg-white shadow-lg fixed w-full top-0 z-50">
            <div class="max-w-7xl mx-auto px-4">
                <div class="flex justify-between h-16">

                    <div class="flex items-center space-x-2">
                        <img src="logo.jpg" alt="Logo" class="h-8 w-auto" />
                        <h1 class="text-heading text-xl font-bold">{ "Dorrego" }</h1>
                    </div>

                    <div class="hidden md:flex items-center space-x-4">
                        <Link<Router> to={Router::News} classes="menu-item">{ "Noticias" }</Link<Router>>
                        <Link<Router> to={Router::Report} classes="menu-item">{ "Denuncias" }</Link<Router>>
                        <Link<Router> to={Router::Project} classes="menu-item">{ "Proyectos" }</Link<Router>>
                    </div>

                    <div class="md:hidden flex items-center">
                        <button id="mobile-menu-button" class="text-gray-600 hover:text-gray-900" onclick={ toggle_menu }>
                            <svg class="h-6 w-6" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2"
                                    d="M4 6h16M4 12h16M4 18h16" />
                            </svg>
                        </button>
                    </div>
                </div>
            </div>

            <div id="mobile-menu" class={ classes!("md:hidden", if *is_hidden { "" } else { "hidden" }) }>
                <div class="px-2 pt-2 pb-3 space-y-1">
                    <Link<Router> to={Router::News} classes="block menu-item">{ "Noticias" }</Link<Router>>
                    <Link<Router> to={Router::Report} classes="block menu-item">{ "Denuncias" }</Link<Router>>
                    <Link<Router> to={Router::Project} classes="block menu-item">{ "Proyectos" }</Link<Router>>
                </div>
            </div>
        </nav>
    }
}
