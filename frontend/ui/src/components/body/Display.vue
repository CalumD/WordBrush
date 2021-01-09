<template>
    <div id="display_area">
        <WordImage :imgData=img v-for="img in images"/>
    </div>
</template>


<script>
import WordImage from "./WordImage";

export default {
    name: 'Display',
    components: {WordImage},
    data() {
        return {
            images: [
                {
                    resultsID: '458ddcfd-7212-470e-9036-bbfb40a35f88',
                    imageID: '0.svg',
                    imageText: '6785'
                },
                {
                    resultsID: '458ddcfd-7212-470e-9036-bbfb40a35f88',
                    imageID: '1.svg',
                    imageText: '=1abcdef!'
                },
                {
                    resultsID: '458ddcfd-7212-470e-9036-bbfb40a35f88',
                    imageID: '2.svg',
                    imageText: 'The'
                },
                {
                    resultsID: '458ddcfd-7212-470e-9036-bbfb40a35f88',
                    imageID: '3.svg',
                    imageText: 'quick'
                },
                {
                    resultsID: '458ddcfd-7212-470e-9036-bbfb40a35f88',
                    imageID: '4.svg',
                    imageText: 'brown'
                },
                {
                    resultsID: '458ddcfd-7212-470e-9036-bbfb40a35f88',
                    imageID: '5.svg',
                    imageText: 'fox'
                },
                {
                    resultsID: '458ddcfd-7212-470e-9036-bbfb40a35f88',
                    imageID: '6.svg',
                    imageText: 'jumps'
                },
                {
                    resultsID: '458ddcfd-7212-470e-9036-bbfb40a35f88',
                    imageID: '7.svg',
                    imageText: 'over'
                },
                {
                    resultsID: '458ddcfd-7212-470e-9036-bbfb40a35f88',
                    imageID: '8.svg',
                    imageText: 'the'
                },
                {
                    resultsID: '458ddcfd-7212-470e-9036-bbfb40a35f88',
                    imageID: '9.svg',
                    imageText: 'lazy'
                },
                {
                    resultsID: '458ddcfd-7212-470e-9036-bbfb40a35f88',
                    imageID: '10.svg',
                    imageText: 'dog'
                }
            ]
        }
    }
}
</script>


<style scoped>
#display_area {
    height: 100%;
    background: #aaa5bd;
    grid-area: display_area;
    padding: 25px;
    align-items: center;
    justify-content: center;
    display: flex;
    flex-wrap: wrap;
    overflow-y: auto;
    overflow-x: hidden;
}
</style>