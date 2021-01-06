<template>
    <div>
        <img id="wb_logo" alt="WordBrush written in WordBrush" src="../assets/WordBrush.svg">
        <!--        <WordImage v-for="x in [1, 2, 'test text', 3]" :valpoop=x></WordImage>-->
    </div>
</template>

<script>
import WordImage from "@/components/WordImage";

export default {
    name: 'ToolBar',
    components: {
        WordImage
    },
    props: [],
    data() {
        return {}
    },
    methods: {
        method_1: function (arg_1) {

        }
    },
    computed: {}
}

</script>

<style scoped>
#wb_logo {
    float: left;
}

</style>