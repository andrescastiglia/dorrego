module.exports = {
    mode: "jit",
    content: [
        "./index.html",
        "src/**/*.rs",
    ],
    theme: {
        extend: {
            colors: {
                'primary': '#98D8AA',
                'secondary': '#E6E6FA',
                'background': '#E6F3FF',
                'heading': '#4A4A4A',
                'text': '#333333'
            }
        },
    },
    plugins: [],
}