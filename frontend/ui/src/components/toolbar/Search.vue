<template>
    <div id="search_container">
        <form id="search_form" style="width: 100%; height: 100%">
            <div class="search_bar_wrapper">
                <!--                    <label>-->
                <input v-model="input_text" placeholder="Enter your words here :)" spellcheck="true" type="text">
                <!--                    </label>-->
                <button id="search_button" type="button">
                    <i class="fa fa-search"></i>
                </button>
            </div>

            <div class="output_options_wrapper">
                <label class="number_chooser">
                    Width:
                    <input v-model="img_width" placeholder="Enter your words here :)" type="number">
                </label>
                <label class="number_chooser">
                    Height:
                    <input v-model="img_height" placeholder="Enter your words here :)" type="number">
                </label>
                <label>
                    Enable Single Image:
                    <input id="enableSFO" name="enableSFO" type="checkbox">
                </label>
                <label class="number_chooser">
                    Single Image Columns:
                    <input v-model="img_sfo" placeholder="Enter your words here :)" type="number">
                </label>
            </div>

            <div class="file_upload_wrapper">
                <input name="input_txt_file" type="file"/>
            </div>
        </form>
    </div>
</template>

<script>
export default {
    name: "Search",
    data() {
        return {
            input_text: '',
            img_width: 0,
            img_height: 0,
            img_sfo: 0,
        }
    }
}
</script>

<style scoped>
#search_container, #search_container form {
    display: flex;
}

#search_container {
    flex-flow: row wrap;
    align-content: stretch;
    justify-content: center;
    flex: 8 1;
    display: flex;
}

.search_bar_wrapper {
    flex-flow: row wrap;
    flex-grow: 3;
    align-items: flex-start;
    align-self: flex-start;
    width: 728px;
}

.output_options_wrapper {
    flex-flow: column wrap;
    align-items: flex-end;
    align-self: center;
    flex-grow: 2;
}

.file_upload_wrapper {
    flex-flow: row wrap;
    align-items: center;
    align-self: center;
    flex-grow: 1;
}

#search_button {
    width: 50px;
    height: 50px;
}
</style>