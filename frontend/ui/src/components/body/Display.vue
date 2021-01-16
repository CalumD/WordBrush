<template>
    <div id="display_area">
        <div v-if="Object.keys(images).length > 0" class="image_output">
            <div v-if="outputType === 'multi'" class="image_output multi_image_output">
                <WordImage
                    :style="{width: `${minImageWidth}px`}"
                    :imgData="{resultsID:resultsID, imageID:key, imageText:[val]}" v-for="(val, key) in images"/>
            </div>
            <div v-else class="image_output single_image_output">
                <WordImage :imgData="{resultsID:resultsID, imageID:key, imageText:singleFileText(val)}"
                           v-for="(val, key) in images"/>
            </div>
        </div>
        <div v-else class="image_output">
            <h1>Enter words above to see the output here!</h1>
        </div>
    </div>
</template>


<script>
import WordImage from "./WordImage";
import {useCurrentResultData} from "@/components/util/useCurrentResultData";

export default {
    name: 'Display',
    components: {WordImage},
    setup() {
        return useCurrentResultData();
    },
    data() {
        return {
            minImageWidth: 280
        }
    },
    computed: {
        images() {
            return ((Object.keys(this.getCurrentResult().images).length > 50)
                ? Object.fromEntries(Object.entries(this.getCurrentResult().images).slice(0, 100)) // TODO: Need to make this flexible to be able to select later images in the result set
                : this.getCurrentResult().images);
        },
        resultsID() {
            return this.getCurrentResult().resultID;
        },
        outputType() {
            return this.getCurrentResult().outputType;
        }
    },
    methods: {
        singleFileText: function (text) {
            const output = [];
            text.forEach(
                line => output.push(line.join(', '))
            )
            return output;
        }
    }
}
</script>


<style scoped>
#display_area {
    height: 100%;
    background: #aaa5bd;
    display: flex;
    align-items: center;
    justify-content: center;
    /*grid-area: display_area;*/
}

.image_output h1 {
    opacity: 20%;
    font-size: xxx-large;
}

.image_output {
    width: 100%;
    height: 100%;
    padding: 25px;
    align-items: center;
    justify-content: center;
    display: flex;
    flex-wrap: wrap;
    overflow-y: auto;
    overflow-x: hidden;
}

.single_image_output .image_data {
    width: 100%;
    height: 100%;
}

.multi_image_output .image_data {
    /*min-width: 300px;*/
    /*min-height: 100px;*/
}
</style>