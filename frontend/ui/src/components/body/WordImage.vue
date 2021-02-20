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
import {useShowAllWordText} from "@/components/util/useShowAllWordText";
import {useColourPaths} from "@/components/util/useColourPaths";
import {useUniqueId} from "@/components/util/useUniqueId";
import {BASE_URL} from '@/main';
import axios from "axios";
import GradientPath from 'gradient-path'

const fill = [
    {color: '#ff7a7a', pos: 0},
    {color: '#fdb57e', pos: 0.25},
    {color: '#f7e279', pos: 0.5},
    {color: '#c6f87f', pos: 0.75},
    {color: '#81ff78', pos: 1}
];

export default {
    name: 'WordImage',
    props: ['imgData'],
    data() {
        return {
            BASE_URL: BASE_URL,
            uniqueID: useUniqueId().uniqueIdValue(),
            svg: "<h1> WordBrush Loading... </h1>",
            drawWithColour: useColourPaths().drawWithColour
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
        },
        'drawWithColour': {
            handler: function (val, oldVal) {
                this.getImageData();
            }
        }
    },
    methods: {
        getImageData: function () {
            this.svg = "<h1> WordBrush Loading... </h1>";
            axios
                .get(`//${BASE_URL}/api/v1/results/${this.imgData.resultsID}/${this.imgData.imageID}`)
                .then((res) => {
                    this.svg = res.data;
                    if (this.drawWithColour) {
                        this.$nextTick(this.colouriseWordBrushPaths);
                    }
                })
                .catch((err) => {
                    console.error(err)
                    this.svg = "<h1> WordBrush Failed! </h1>"
                });
        },
        colouriseWordBrushPaths: function () {
            let gradientPath;

            document.querySelectorAll(`#${this.uniqueID} svg path`)
                .forEach(path => {
                    gradientPath = new GradientPath({
                        path: path,
                        segments: 100,
                        samples: 5
                    });

                    gradientPath.render({
                        type: 'circle',
                        fill: fill,
                        width: 5,
                        stroke: fill,
                        strokeWidth: 0.5
                    });
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