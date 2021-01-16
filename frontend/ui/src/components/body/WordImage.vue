<template>
    <div :style="{
            'background-image': `url(//${BASE_URL}/api/v1/results/${imgData.resultsID}/${imgData.imageID})`,
         }"
         class="image_data"
    >
        <div :class="{reveal_all : forceDisplayOfText}"
             class="image_overlay word_reveal">
            <h1 v-for="word_or_line in imgData.imageText">
                {{ word_or_line }}
            </h1>
        </div>
    </div>
</template>

<script>
import {useShowAllWordText} from "@/components/util/showAllWordText";
import {BASE_URL} from '@/main';

export default {
    name: 'WordImage',
    props: ['imgData'],
    data() {
        return {
            BASE_URL: BASE_URL
        }
    },
    setup() {
        return useShowAllWordText();
    },
    computed: {
        forceDisplayOfText() {
            return this.showAllWordText;
        }
    }
}
</script>

<style scoped>
.image_data {
    background-size: contain;
    background-repeat: no-repeat;
    background-position: center;

    min-width: 100px;
    max-width: 90vw;

    min-height: 100px;
    max-height: 90vh;

    margin: 10px;
}

.image_overlay {
    opacity: 0;
    width: 100%;
    height: 100%;
    padding: 10px;
    transition: opacity .2s linear;
    text-align: center;
    display: grid;
}

.word_reveal:hover, .reveal_all {
    opacity: 0.7;
}

.image_overlay h1 {
    font-size: 300%;
    font-weight: bold;
}
</style>