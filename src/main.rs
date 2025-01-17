mod components;

use components::*;
use yew::prelude::*;

#[function_component]
pub fn App() -> Html {
    html! {
        <div class="bg-pattern">
            <div class="min-h-screen flex flex-col">
                <TagBody />
                <TagFooter />
            </div>
        </div>
    }
}

pub fn main() {
    yew::Renderer::<App>::new().render();
}
