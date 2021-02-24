<template>
    <div id="display_area">
        <div v-if="Object.keys(images).length > 0" class="image_output">
            <div v-if="outputType === 'multi'" class="multi_image_output box">
                <WordImage
                    :imgData="{
                    resultsID:resultsID,
                    imageID:key,
                    imageText:[val]
                }" v-for="(val, key) in images"/>
            </div>
            <div v-else class="single_image_output">
                <WordImage :imgData="{
                    resultsID:resultsID,
                    imageID:key,
                    imageText:singleFileText(val)
                }" v-for="(val, key) in images"/>
            </div>
        </div>
        <div v-else class="image_output" style="text-align: center">
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
}

.image_output h1:before {
    content: '';
    display: inline-block;
    height: 100%;
    vertical-align: middle;
    margin-right: -0.25em;
}

.image_output h1 {
    opacity: 0.2;
    font-size: xxx-large;

    display: inline-block;
    vertical-align: middle;
    height: 100%;
}

.image_output {
    width: 100%;
    height: 100%;
}

.box {
    width: 100%;
    height: 100%;
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(20rem, 1fr));
    grid-auto-flow: dense;
}

.single_image_output,
.single_image_output .image_data {
    width: 100%;
    height: 100%;
}

.single_image_output {
    padding: 25px;
    align-items: stretch;
    justify-content: center;
    display: flex;
    flex-wrap: wrap;
    overflow-y: auto;
    overflow-x: hidden;
}
</style>