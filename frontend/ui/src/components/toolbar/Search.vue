<template>
    <div id="search_container">
        <form id="search_form" style="width: 100%; height: 100%" onsubmit="return false">

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

            <div class="file_upload_wrapper">
                <div class="file_upload_wrapper_ticky_wrapper">
                    <p>Upload File</p>
                    <label class="ticky_box">
                        <input type="checkbox" v-model="form.enable_file"/>
                    </label>
                    <p
                        id="file_name"
                        :style="{opacity: (form.enable_file ? 1 : 0.4)}"
                    >
                        {{ selected_file_text }}
                    </p>
                </div>
                <input id="file_upload_input" type="file" @change="handleFileSelection" style="display: none;"
                       :disabled="!form.enable_file"/>
                <label class="file_upload_button" for="file_upload_input">
                    <FontAwesomeIcon icon="upload"/>
                </label>
            </div>

            <SearchOptions v-model:opts="form.options"/>
        </form>
        <Modal
            v-if="displayErrorModal"
            v-model:show="shouldError"
            :heading="errorData.message"
            :body="`${errorData.description}${
                errorData.data
                ? '\n' + (errorData.data.stdErr ? errorData.data.stdErr : JSON.stringify(errorData.data))
                : ''
            }`"
        />
    </div>
</template>

<script>
import FontAwesomeIcon from "@/components/util/FontAwesomeIcon";
import axios from 'axios';
import SearchOptions from "@/components/toolbar/SearchOptions";
import {BASE_URL} from '@/main';
import {useCurrentResultData} from "@/components/util/useCurrentResultData";
import Modal from "@/components/util/Modal";

export default {
    name: "Search",
    components: {SearchOptions, FontAwesomeIcon, Modal},
    data() {
        return {
            form: {
                text: '',
                options: {
                    width: 0,
                    height: 0,
                    sfo: 0,
                    traceColour: false
                },
                enable_file: false,
                file: undefined
            },
            BASE_URL: BASE_URL,
            shouldError: false,
            errorData: {}
        }
    },
    setup() {
        return useCurrentResultData();
    },
    methods: {
        executeSearch: function () {
            console.log("should now be executing search", this.form);

            let queryParams = ''
            queryParams += (this.form.text ? `input=${this.form.text}` : '');
            queryParams += (this.form.options.width ? `&w=${this.form.options.width}` : '');
            queryParams += (this.form.options.height ? `&h=${this.form.options.height}` : '');
            queryParams += (this.form.options.sfo ? `&sfo=${this.form.options.sfo}` : '');

            if (this.form.enable_file && this.form.file) {
                const fileToUp = new FormData();
                fileToUp.append('input_txt_file', this.form.file);
                console.log("CALLING THE POSTER");
                axios
                    .post(`http://${this.BASE_URL}/api/v1/words${queryParams.length > 0 ? `?${queryParams}` : ''}`, fileToUp,
                        {
                            headers: {
                                "Content-Type": "multipart/form-data"
                            }
                        })
                    .then((res) => {
                        console.log(res);
                        if (res.status === 200) {
                            this.fetchNewResultSet(res.data.resultSetID);
                        } else {
                            console.error(res);
                        }
                        console.log("SUCCEEDED POSTER");
                    })
                    .catch((err) => this.setAxiosError(err));
            } else {
                console.log("CALLING THE GETTER");
                axios
                    .get(`http://${this.BASE_URL}/api/v1/words${queryParams.length > 0 ? `?${queryParams}` : ''}`)
                    .then((res) => {
                        console.log(res);
                        if (res.status === 200) {
                            this.fetchNewResultSet(res.data.resultSetID);
                        } else {
                            console.error(res);
                        }
                        console.log("SUCCEEDED GETTER");
                    })
                    .catch((err) => this.setAxiosError(err));
            }
        },
        handleFileSelection: function (event) {
            this.form.file = event.target.files[0];
        }
    },
    computed: {
        selected_file_text: function () {
            if (this.form.file) {
                if (this.form.file.name.length > 16) {
                    return `(${this.form.file.name.substring(0, 8)}[...]${this.form.file.name.slice(-5)})`;
                }
                return `(${this.form.file.name})`;
            } else {
                return '(No file selected)';
            }
        },
        displayErrorModal: function () {
            this.errorData = this.getCurrentResult().error;
            this.shouldError = !!this.errorData.message;
            return this.shouldError;
        }
    },
    watch: {
        'shouldError': {
            handler: function (val, oldVal) {
                if (!val) {
                    this.acknowledgeError();
                }
            }
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
    /*max-width: 800px;*/
    justify-content: center;
}

.search_bar_wrapper:hover input,
.search_bar_wrapper:hover button,
#search_bar:focus,
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
    outline: none;
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

.ticky_box input {
    transform: scale(1.2);
}

.file_upload_wrapper {
    display: flex;
    flex-flow: row;
    align-items: center;
    justify-content: center;
    flex: 2 2;
    min-width: 100px;
    height: 100%;
    color: white;
}

.file_upload_wrapper_ticky_wrapper {
    display: flex;
    flex-flow: column;
    align-items: center;
    justify-content: center;
    width: 100%;
    height: 100%;
    color: white;
}

.file_upload_button {
    display: flex;
    align-items: center;
    justify-content: center;
    height: 60%;
    width: 60%;
    outline: none;
    background-color: #e3e3e3;
    color: black;
    border-color: black;
    border-radius: 10px;
    margin-left: 10px;
}

#file_upload_input:disabled ~ .file_upload_button svg {
    opacity: 20%;
}

.file_upload_button svg {
    height: 75%;
    width: 75%;
}


</style>