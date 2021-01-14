<template>
    <div id="display_area">
        <button type="button" @click="toggg" style="width: 100px; height: 100px;"></button>
        <WordImage :imgData="{resultsID:resultsID, ...img}" v-for="img in images"/>
    </div>
</template>


<script>
import WordImage from "./WordImage";
import {useCurrentResultData} from "@/components/util/useCurrentResultData";

let whichOne = true;

export default {
    name: 'Display',
    components: {WordImage},
    setup() {
        return useCurrentResultData();
    },
    computed: {
        images() {
            return this.getCurrentResult().images;
        },
        resultsID() {
            return this.getCurrentResult().resultID;
        }
    },
    methods: {
        toggg: function () {
            whichOne = !whichOne;

            if (whichOne) {
                this.search();
            } else {
                this.search2Tho();
            }
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