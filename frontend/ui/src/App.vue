<template>
    <div class="whole_screen">
        <ToolBar id="toolbar" @logo-hovered="logoHovered" @logo-not-hovered="logoNotHovered"></ToolBar>
        <Display id="display_area" :showAllWordText="showAllWordText"></Display>
    </div>
</template>

<script>
import ToolBar from "./components/ToolBar";
import Display from "./components/Display";

export default {
    name: 'App',
    components: {
        Display,
        ToolBar
    },
    data() {
        return {
            showAllWordText: false
        }
    },
    methods: {
        logoHovered: function (params) {
            this.showAllWordText = params;
        },
        logoNotHovered: function (params) {
            this.showAllWordText = params;
        }
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
    /*text-align: center;*/
    color: #2c3e50;
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
    grid-template-rows: 70px auto;
    grid-template-columns: auto;
    min-height: 400px;
    min-width: 400px;
}

#toolbar {
    height: 100%;
    background: red;
    grid-area: toolbar;
}

#display_area {
    height: 100%;
    background: blue;
    grid-area: display_area;
    padding: 25px;
}

*, ::before, ::after {
    box-sizing: border-box;
}
</style>
