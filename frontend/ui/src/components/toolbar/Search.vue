<template>
    <div id="search_container">
        <form id="search_form" style="width: 100%; height: 100%">
            <div class="search_bar_wrapper">
                <label for="search_bar"/>
                <input id="search_bar" class="search_bar" v-model="input_text" placeholder="Enter your words here :)"
                       spellcheck="true" type="text">
                <div v-if="input_text.length > 0">
                    X
                </div>
                <span></span>
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
    flex-flow: row wrap;
}

#search_container {
    /*align-content: stretch;*/
    justify-content: center;
    flex: 8 1;
    display: flex;
}

.search_bar_wrapper {
    display: flex;
    flex-flow: row nowrap;
    flex-grow: 3;
    align-items: flex-start;
    align-self: flex-start;
    max-width: 800px;
}

.search_bar_wrapper:hover input,
.search_bar_wrapper:hover button,
#search_bar:focus, #search_button:focus,
#search_bar:focus ~ #search_button,
#search_button:focus ~ #search_bar {
    box-shadow: 5px 5px 12px red;
}

label.search_bar {
    width: 0;
}

input.search_bar {
    padding-left: 10px;
    padding-right: 10px;
    font-size: 1.25rem;
}

.search_bar {
    min-width: 728px;
    max-width: 750px;
    display: flex;
    height: 50px;
    border-top-left-radius: 25px;
    border-bottom-left-radius: 25px;
    outline: none;
}

.search_bar_wrapper span {
    height: 65%;
    border-left: 1px solid #404040;
    opacity: 0.8;
}

#search_button {
    width: 49px;
    height: 50px;
    background-color: white;
    border-top-right-radius: 25px;
    border-bottom-right-radius: 25px;
}


.output_options_wrapper {
    flex-flow: column wrap;
    align-items: flex-end;
    align-self: center;
    flex-grow: 2;
    min-width: 100px;
    max-width: 100px;
}

.file_upload_wrapper {
    flex-flow: row wrap;
    align-items: center;
    align-self: center;
    flex-grow: 1;
    max-width: 100px;
}
</style>