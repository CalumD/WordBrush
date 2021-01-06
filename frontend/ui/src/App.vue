<template>
    <div>
        <ToolBar id="toolbar"></ToolBar>
        <Display id="display_area"></Display>
    </div>
</template>

<script>
import ToolBar from "@/components/ToolBar";
import Display from "@/components/Display";

export default {
    name: 'App',
    components: {
        Display,
        ToolBar
    },
}

</script>

<style>
/* Apply a CSS reset */
html, body, div, span, applet, object, iframe,
h1, h2, h3, h4, h5, h6, p, blockquote, pre,
a, abbr, acronym, address, big, cite, code,
del, dfn, em, img, ins, kbd, q, s, samp,
small, strike, strong, sub, sup, tt, var,
b, u, i, center,
dl, dt, dd, ol, ul, li,
fieldset, form, label, legend,
table, caption, tbody, tfoot, thead, tr, th, td,
article, aside, canvas, details, embed,
figure, figcaption, footer, header, hgroup,
menu, nav, output, ruby, section, summary,
time, mark, audio, video {
    margin: 0;
    padding: 0;
    border: 0;
    font: inherit;
    font-size: 100%;
    vertical-align: baseline;
}

#app {
    font-family: Avenir, Helvetica, Arial, sans-serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    text-align: center;
    color: #2c3e50;
}

#toolbar {
    background-color: red;
    min-width: 100vw;
    min-height: 10vh;
    max-height: 25vh;
}

#display_area {
    background-color: green;
    min-width: 100vw;
    min-height: 10vh;
    max-height: 75vh;
}
</style>
