<template>
    <div class="main_toolbar_container">

        <div class="logo_container" @mouseenter="alertImagesHovered" @mouseleave="alertImagesNotHovered">
            <img id="wb_logo" alt="WordBrush written in WordBrush as logo." src="../assets/WordBrush.svg">
            <h1>WordBrush</h1>
        </div>

        <div class="search_container">
            <form>
                <label>
                    <input v-model="input_text" placeholder="Enter your words here :)" type="text">
                </label>
                <button type="button">
                    <i class="fa fa-search"></i>
                </button>
                <label>
                    W:
                    <input v-model="img_width" placeholder="Enter your words here :)" type="number">
                </label>
                <label>
                    H:
                    <input v-model="img_height" placeholder="Enter your words here :)" type="number">
                </label>
                <label>
                    SFO:
                    <input v-model="img_sfo" placeholder="Enter your words here :)" type="number">
                </label>
            </form>
        </div>

        <div class="previous_container">
            <p> {{ existing_outputs }} </p>
        </div>
    </div>
</template>

<script>
import WordImage from "../components/WordImage";
import axios from 'axios';

export default {
    name: 'ToolBar',
    components: {
        WordImage
    },
    props: [],
    data() {
        return {
            input_text: '',
            img_width: 0,
            img_height: 0,
            img_sfo: 0,
            existing_outputs: []
        }
    },
    methods: {
        alertImagesHovered: function () {
            this.$emit("logo-hovered", true);
        },
        alertImagesNotHovered: function () {
            this.$emit("logo-not-hovered", false);
        },
        fetchExistingOutput: function () {
            axios
                .get('http://ubuntu.vms.local:3000/api/v1/results')
                .then((out) => {
                    this.existing_outputs = out.data
                });
        }
    },
    computed: {},
    mounted() {
        this.fetchExistingOutput();
    }
}

</script>

<style scoped>
.main_toolbar_container, .logo_container, .search_container, .previous_container {
    display: flex;
}

.main_toolbar_container {
    flex-flow: row wrap;
    align-items: center;
    align-self: center;
}

.main_toolbar_container > .logo_container, .search_container, .previous_container {
    height: 100%;
    padding: 5px;
    align-self: inherit;
}

.logo_container {
    justify-content: flex-start;
    flex: 1 4;
}

.logo_container h1 {
    align-self: inherit;
}

.search_container {
    align-content: stretch;
    justify-content: center;
    flex: 8 1;
}

.previous_container {
    justify-content: flex-end;
    flex: 0.5 4;
    max-height: 100%;
}

#wb_logo {
    max-height: 100%;
}

</style>