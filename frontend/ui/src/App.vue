<template>
    <div class="whole_screen">
        <ToolBar/>
        <Display/>
    </div>
    <div id="modals"/>
</template>


<script>
import ToolBar from "./components/toolbar/ToolBar";
import Display from "./components/body/Display";

export default {
    name: 'App',
    components: {
        Display,
        ToolBar
    }
}
</script>


<style>
/* Apply a CSS reset */
* {
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
}

div {
    display: block;
}

.whole_screen {
    display: grid;
    height: 100vh;
    grid-template-areas:
        'toolbar'
        'display_area';
    grid-gap: 0;
    grid-template-rows: 120px auto;
    grid-template-columns: auto;
    min-height: 400px;
    min-width: 400px;
}


*, ::before, ::after {
    box-sizing: border-box;
}

button, input, #file_upload_input:enabled ~ .file_upload_button, .button_like {
    cursor: pointer;
}
</style>
