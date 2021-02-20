<template>
    <div class="image_data">
        <div
            v-html="svg"
            :id="uniqueID"
        />
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
import {useUniqueId} from "@/components/util/useUniqueId";
import axios from "axios";

export default {
    name: 'WordImage',
    props: ['imgData'],
    data() {
        return {
            BASE_URL: BASE_URL,
            uniqueID: useUniqueId().uniqueIdValue(),
            svg: "<h1> WordBrush Loading... </h1>",
            triggerOnce: true
        }
    },
    setup() {
        return useShowAllWordText();
    },
    computed: {
        forceDisplayOfText() {
            return this.showAllWordText;
        }
    },
    beforeMount() {
        this.getImageData()
    },
    watch: {
        'imgData': {
            handler: function (val, oldVal) {
                this.getImageData();
            }
        }
    },
    methods: {
        getImageData: function () {
            axios
                .get(`//${BASE_URL}/api/v1/results/${this.imgData.resultsID}/${this.imgData.imageID}`)
                .then((res) => {
                    this.svg = res.data;
                })
                .catch((err) => {
                    console.error(err)
                    this.svg = "<h1> WordBrush Failed! </h1>"
                });
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

    min-height: 100px;
    max-height: 90vh;

    margin: 10px;
    position: relative;

    flex-grow: 1;
}

.image_overlay {
    opacity: 0;
    width: 100%;
    height: 100%;
    padding: 10px;
    transition: opacity .2s linear;
    text-align: center;
    display: grid;
    position: absolute;
    z-index: 10;
}

.word_reveal:hover, .reveal_all {
    opacity: 0.7;
}

.image_overlay h1 {
    font-size: 300%;
    font-weight: bold;
    align-self: center;
}
</style>