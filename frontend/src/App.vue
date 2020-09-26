<template>
    <div id="app">
        <img alt="WordBrush Image" src="./assets/test.svg">
        <WordBrush :msg="message"/>
    </div>
</template>

<script>
import WordBrush from './components/WordBrush.vue'

export default {
    name: 'app',
    components: {
        WordBrush
    },
    data() {
        return {
            message: 'Welcome to word brush App!'
        }
    }
}
</script>

<style>
#app {
    font-family: 'Avenir', Helvetica, Arial, sans-serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    text-align: center;
    color: #2c3e50;
    margin-top: 60px;
}
</style>