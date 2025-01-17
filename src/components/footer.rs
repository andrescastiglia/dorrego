use yew::prelude::*;

#[function_component]        
pub fn TagFooter() -> Html {
    html! {
        <footer class="bg-white shadow-lg mt-auto">
            <div class="max-w-7xl mx-auto px-4 py-6">
                <div class="flex flex-col md:flex-row justify-between items-center space-y-4 md:space-y-0">
                    <div class="flex items-center space-x-4">
                        <a href="mailto:contacto@dorrego.com" class="text-text hover:text-heading">
                            <span class="mr-2">{ "✉️" }</span>{ "contacto@dorrego.com" }
                        </a>
                    </div>
                    <div class="flex items-center space-x-4">
                        <a href="https://wa.me/1234567890" class="text-text hover:text-heading" target="_blank"
                            rel="noopener noreferrer">
                            <span class="mr-2">{ "📱" }</span>{ "WhatsApp: +54 123-456-7890" }
                        </a>
                    </div>
                </div>
            </div>
        </footer>
    }
}
