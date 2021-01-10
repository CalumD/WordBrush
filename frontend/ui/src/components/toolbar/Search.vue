<template>
    <div id="search_container">
        <form id="search_form" style="width: 100%; height: 100%">
            <div class="search_bar_wrapper">
                <label for="search_bar"/>
                <input
                    @keyup.enter="executeSearch"
                    id="search_bar" class="search_bar" v-model.trim="form.text" placeholder="Enter your words here :)"
                    spellcheck="true" type="text"
                />
                <FontAwesomeIcon v-if="form.text.length > 0" class="icons" id="search_bar_cross" icon="times"
                                 @click="form.text = ''"/>
                <span></span>
                <button @click="executeSearch" id="search_button" type="button">
                    <FontAwesomeIcon class="icons" id="search_button_icon" icon="search"/>
                </button>
            </div>

            <div class="output_options_wrapper">

                <NumberChooser :min="50" :max="3840" v-model:result="form.width"></NumberChooser>
                <NumberChooser :min="50" :max="3840" v-model:result="form.height"></NumberChooser>

                <label class="ticky_box">
                    Enable Single Image:
                    <input type="checkbox" v-model="form.enable_sfo"/>
                </label>
                <label v-if="form.enable_sfo" class="number_chooser">
                    <NumberChooser :min="1" :max="100" v-model:result="form.sfo"></NumberChooser>
                </label>

                <label class="ticky_box">
                    Upload Input File in request:
                    <input type="checkbox" v-model="form.enable_file"/>
                </label>
            </div>

            <div v-if="form.enable_file" class="file_upload_wrapper">
                <input type="file" @change="handleFileSelection"/>
            </div>
        </form>
    </div>
</template>

<script>
import FontAwesomeIcon from "@/components/util/FontAwesomeIcon";
import axios from 'axios';
import NumberChooser from "@/components/util/NumberChooser";

export default {
    name: "Search",
    components: {NumberChooser, FontAwesomeIcon},
    data() {
        return {
            form: {
                text: '',
                width: 0,
                height: 0,
                enable_sfo: false,
                sfo: 0,
                enable_file: false,
                file: undefined
            }
        }
    },
    methods: {
        executeSearch: function () {
            console.log("should now be executing search", this.form);

            let queryParams = ''
            queryParams += (this.form.text ? `input=${this.form.text}` : '');
            queryParams += (this.form.width ? `&w=${this.form.width}` : '');
            queryParams += (this.form.height ? `&h=${this.form.height}` : '');
            queryParams += (this.form.sfo ? `&sfo=${this.form.sfo}` : '');

            if (this.form.enable_file && this.form.file) {
                const fileToUp = new FormData();
                fileToUp.append('input_txt_file', this.form.file[0]);
                console.log("CALLING THE POSTER");
                axios
                    .post(`http://ubuntu.vms.local:3000/api/v1/words${queryParams.length > 0 ? `?${queryParams}` : ''}`, fileToUp,
                        {
                            headers: {
                                "Content-Type": "multipart/form-data"
                            }
                        })
                    .then((res) => {
                        console.log(res);
                        console.log("SUCCEEDED POSTER");
                    })
                    .catch(() => {
                        console.log("FAILED POSTER");
                    });
            } else {
                console.log("CALLING THE GETTER");
                axios
                    .get(`http://ubuntu.vms.local:3000/api/v1/words${queryParams.length > 0 ? `?${queryParams}` : ''}`)
                    .then((res) => {
                        console.log(res);
                        console.log("SUCCEEDED GETTER");
                    })
                    .catch(() => {
                        console.log("FAILED GETTER");
                    });
            }

        },
        handleFileSelection: function (event) {
            this.form.file = event.target.files
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
    align-self: center;
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
    padding-right: 40px;
    font-size: 1.25rem;
}

.search_bar {
    min-width: 700px;
    width: 100%;
    max-width: 750px;
    display: flex;
    height: 50px;
    border-top-left-radius: 25px;
    border-bottom-left-radius: 25px;
    outline: none;
}

.search_bar_wrapper span {
    height: 70%;
    border-left: 1px solid #7b7b7b;
    margin-left: -2px;
}

#search_button {
    width: 49px;
    height: 50px;
    background-color: white;
    border-top-right-radius: 25px;
    border-bottom-right-radius: 25px;
    align-items: center;
    display: flex;
    justify-content: center;
}

#search_button_icon {
    margin-left: -3px;
}

#search_bar_cross {
    margin-left: -40px;
    align-self: center;
    width: 40px;
    height: 40px;
    opacity: 0.5;
}

.icons {
    display: flex;
    width: 60%;
    height: 60%;
    color: #404040;
}

.output_options_wrapper {
    flex-flow: column wrap;
    align-items: flex-end;
    align-self: center;
    flex-grow: 2;
    min-width: 100px;
    max-width: 100px;
    height: 100%;
}

.number_chooser {
    width: 4rem;
}

.number_chooser input {
    width: 100%;
}

.file_upload_wrapper {
    flex-flow: row wrap;
    align-items: center;
    align-self: center;
    flex-grow: 1;
    max-width: 100px;
}
</style>