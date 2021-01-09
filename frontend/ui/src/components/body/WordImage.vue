<template>
    <div :style="{
            'background-image': `url(//ubuntu.vms.local:3000/api/v1/results/${imgData.resultsID}/${imgData.imageID})`,
            'background-size': 'contain',
            'background-repeat': 'no-repeat',
            'background-position': 'center'
         }"
         class="image_data"
    >
        <div :class="{reveal_all : forceDisplayOfText}"
             class="image_overlay word_reveal">
            <h1>
                {{ imgData.imageText }}
            </h1>
        </div>
    </div>
</template>

<script>
import {useShowAllWordText} from "@/components/util/showAllWordText";

export default {
    name: 'WordImage',
    props: ['imgData'],
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
    min-width: 100px;
    max-width: 1000px;

    min-height: 100px;
    max-height: 1000px;

    margin: 10px;

    width: calc(100vw / 6);
}

.image_overlay {
    opacity: 0;
    width: 100%;
    height: 100%;
    padding: 10px;
    transition: opacity .2s linear;
    text-align: center;
}

.word_reveal:hover, .reveal_all {
    opacity: 0.7;
}

.image_overlay h1 {
    font-size: 300%;
    font-weight: bold;
}
</style>