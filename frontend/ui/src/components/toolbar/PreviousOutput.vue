<template>
    <div id="previous_container" v-click-away="close">
        <button type="button" class="history_button" @click="clicked">
            <transition name="history_button_transition" mode="out-in">
                <FontAwesomeIcon v-if="!viewingHistory" class="history_icon" icon="history"></FontAwesomeIcon>
                <FontAwesomeIcon v-else class="history_icon" icon="times"></FontAwesomeIcon>
            </transition>
        </button>
        <transition name="dropdown">
            <div v-if="viewingHistory" class="history_dropdown_wrapper">
                <ul class="history_dropdown_content">
                    <li v-if="existing_outputs.length > 0" v-for="item in existing_outputs">
                        <span>{{ displayData[item] }}</span>
                        <FontAwesomeIcon @click="getPreviousOutput(item)" class="rerun_history_icon"
                                         icon="redo"></FontAwesomeIcon>
                        <span>Rerun</span>
                    </li>
                    <li v-else style="max-width: 100%;">
                        <span>No Past Searches Available.</span>
                    </li>
                </ul>
            </div>
        </transition>
    </div>
</template>

<script>
import FontAwesomeIcon from "@/components/util/FontAwesomeIcon";
import {useCurrentResultData} from "@/components/util/useCurrentResultData";
import axios from "axios";
import {BASE_URL} from "@/main";

export default {
    name: "PreviousOutput",
    components: {FontAwesomeIcon},
    setup() {
        return useCurrentResultData();
    },
    data() {
        return {
            viewingHistory: false,
            existing_outputs: [],
            displayData: {}
        }
    },
    methods: {
        clicked: function () {
            this.viewingHistory = !this.viewingHistory
            if (this.viewingHistory) {
                this.fetchExistingOutput();
            }
        },
        close: function () {
            this.viewingHistory = false;
        },
        getPreviousOutput: function (resultSetID) {
            this.fetchNewResultSet(resultSetID);
            this.viewingHistory = false;
        },
        fetchExistingOutput: function () {
            axios
                .get(`http://${BASE_URL}/api/v1/results`)
                .then((out) => {
                    this.existing_outputs = out.data
                    for (let output of this.existing_outputs) {
                        this.resolveWords(output);
                    }
                })
                .catch((err) => {
                    console.log("Failed to retrieve existing results data", {err: err});
                    this.existing_outputs = [];
                });
        },
        resolveWords: async function (fromSet) {
            let resultSetWord = '[...unknown...]';
            axios
                .get(`http://${BASE_URL}/api/v1/results/${fromSet}`)
                .then((out) => {
                    if (out.data.outputType === 'multi') {
                        resultSetWord = Object.values(out.data.words).join(" ");
                    } else if (out.data.outputType === 'single') {
                        const data = Object.values(out.data.words)[0];
                        const result = [];
                        for (let row = 0; row < 100 && row < data.length; row++) {
                            for (const column in data[row]) {
                                result.push(data[row][column]);
                            }
                        }
                        resultSetWord = Object.values(result).join(" ");
                    } else {
                        console.error('Unsupported word output type: ' + out.data.outputType)
                    }
                })
                .catch((err) => {
                    console.error("Failed to retrieve existing result set data", {err: err});
                })
                .finally(() => {
                    this.displayData[fromSet] = `${this.clipLength(resultSetWord)} (${fromSet.split("-")[0]})`;
                });
        },
        clipLength: function (input) {
            if (input.length > 75) {
                return input.substring(0, 75) + '[...]';
            }
            return input;
        }
    },
    mounted() {
        this.fetchExistingOutput();
    }
}
</script>

<style scoped>

#previous_container {
    display: flex;
    align-items: center;
    justify-content: center;
    flex: 0.5 4;
    max-height: 100%;
}

.history_button {
    height: 60%;
    width: 60%;
    outline: none;
    background-color: #e3e3e3;
    border-color: black;
    border-radius: 10px;
}

.history_icon {
    height: 75%;
    width: 75%;
}

.history_button_transition-enter-active,
.history_button_transition-leave-active {
    transition: all 0.3s ease-out;
}

.history_button_transition-enter-from,
.history_button_transition-leave-to {
    opacity: 0;
}

.dropdown-enter-active,
.dropdown-leave-active {
    transition: all 0.6s;
}

.dropdown-enter-from,
.dropdown-leave-to {
    opacity: 0;
    transform: translateY(30px);
}


.history_dropdown_wrapper {
    display: flow-root;
    position: absolute;
    z-index: 999;
    top: 120px;
    right: 10px;
    height: 50%;
    min-height: 200px;
    max-height: 80%;
    min-width: 300px;
    max-width: 50%;
    border-radius: 12px;
    /*padding: 12px 0 12px 0;*/
    background-color: white;
    border: 2px solid gray;
    overflow-y: hidden;
}

.history_dropdown_content {
    display: flex;
    flex-flow: column nowrap;
    overflow-y: scroll;
    height: 100%;
    max-width: 100%;
    width: auto;
}

.history_dropdown_content li {
    display: flex;
    flex-flow: row nowrap;
    flex-shrink: 1;
    justify-content: flex-end;
    max-height: 30px;
    text-decoration: none;
    padding: 5px 5% 5px 5%;
}


.history_dropdown_wrapper li:nth-child(odd) {
    background: #efefef;
}

.history_dropdown_wrapper li:hover {
    color: #17bf63;
    background-color: rgba(79, 192, 141, 0.3);
    border-radius: 5px;
}

.history_dropdown_wrapper span {
    flex-shrink: 1;
    text-align: left;
    max-width: 80%;
}

.rerun_history_icon {
    max-height: 100%;
    width: 10%;
    max-width: 20%;
    color: #af5a3a;
}

.rerun_history_icon:hover {
    cursor: pointer;
}
</style>