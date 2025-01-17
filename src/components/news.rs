use yew::prelude::*;

#[function_component]        
pub fn TagNews() -> Html {
    html! {
        <section id="noticias" class="mb-12">
            <div class="content-overlay rounded-lg p-6">
                <h2 class="text-2xl font-bold text-heading mb-6">{ "Noticias Recientes" }</h2>
                <div class="grid md:grid-cols-2 lg:grid-cols-3 gap-6">
                    <div class="bg-white rounded-lg shadow-md p-6">
                        <h3 class="text-heading font-semibold mb-2">{ "Noticia 1" }</h3>
                        <p class="text-text mb-4">{ "Descripción breve de la noticia más reciente..." }</p>
                        <button class="primary-button">{ "Leer más" }</button>
                    </div>
                    <div class="bg-white rounded-lg shadow-md p-6">
                        <h3 class="text-heading font-semibold mb-2">{ "Noticia 2" }</h3>
                        <p class="text-text mb-4">{ "Otra noticia importante para la comunidad..." }</p>
                        <button class="primary-button">{ "Leer más" }</button>
                    </div>
                    <div class="bg-white rounded-lg shadow-md p-6">
                        <h3 class="text-heading font-semibold mb-2">{ "Noticia 3" }</h3>
                        <p class="text-text mb-4">{ "Últimas actualizaciones del proyecto..." }</p>
                        <button class="primary-button">{ "Leer más" }</button>
                    </div>
                </div>
                <div class="mt-6 text-center">
                    <a href="#" class="primary-button inline-block">{ "Ver Noticias Anteriores" }</a>
                </div>
            </div>
        </section>
    }
}
