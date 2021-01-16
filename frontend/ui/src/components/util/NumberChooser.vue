<template>
    <label class="number_chooser">
        <input
            class="num_chooser_input"
            type="number"
            pattern="[0-9]*"
            :min="min"
            :max="max"
            :step="step"
            v-model="result"
        />
        <button
            type="button"
            @mousedown="startDecrement"
            @mouseleave="stopChanging"
            @mouseup="stopChanging"
            class="num_chooser_button_minus">
            <FontAwesomeIcon icon="minus"/>
        </button>
        <button
            type="button"
            @mousedown="startIncrement"
            @mouseleave="stopChanging"
            @mouseup="stopChanging"
            class="num_chooser_button_plus">
            <FontAwesomeIcon icon="plus"/>
        </button>
    </label>
</template>

<script>
import {useDuplexData} from "@/components/util/useDuplexData";
import FontAwesomeIcon from "@/components/util/FontAwesomeIcon";

const clickHoldChangeInitialSpeed = 400;    // Higher takes longer before the 'and hold' effect takes place
const clickHoldChangeRate = 1.3;            // Higher means we will reach the highest button speed quicker
const clickHoldChangeMaxSpeed = 5;          // Lower is faster updates at the quickest speed

export default {
    name: "NumberChooser",
    components: {FontAwesomeIcon},
    props: {
        min: {
            type: Number,
            default: 0
        },
        step: {
            type: Number,
            default: 1
        },
        max: {
            type: Number,
            default: 10
        },
        result: Number
    },
    data() {
        return {
            isDecrementing: false,
            isIncrementing: false,
            currentChangeRate: 500
        }
    },
    setup(props, {emit}) {
        return {
            result: useDuplexData(props, emit, {name: 'result', outputMutator: (num) => parseInt(num, 10)})
        }
    },
    methods: {
        updateChangeRate() {
            return ((this.currentChangeRate / clickHoldChangeRate) < clickHoldChangeMaxSpeed)
                ? clickHoldChangeMaxSpeed
                : (this.currentChangeRate / clickHoldChangeRate);
        },
        startDecrement: function () {
            if ((this.result - this.step) >= this.min) {
                if (this.result > this.max) {
                    this.result = this.max;
                } else {
                    this.result -= this.step;
                }
                this.isDecrementing = setTimeout(this.startDecrement, this.currentChangeRate);
                this.currentChangeRate = this.updateChangeRate();
            } else {
                this.result = 0;
                this.stopChanging();
            }
        },
        startIncrement: function () {
            if ((this.result + this.step) <= this.max) {
                if (this.result < this.min) {
                    this.result = this.min;
                } else {
                    this.result += this.step;
                }
                this.isIncrementing = setTimeout(this.startIncrement, this.currentChangeRate);
                this.currentChangeRate = this.updateChangeRate();
            } else {
                this.result = 9999;
                this.stopChanging();
            }
        },
        stopChanging: function () {
            if (this.isDecrementing) {
                clearTimeout(this.isDecrementing);
            }
            if (this.isIncrementing) {
                clearTimeout(this.isIncrementing);
            }
            this.currentChangeRate = clickHoldChangeInitialSpeed;
        }
    }
};

</script>

<style scoped>

.number_chooser {
    display: flex;
    flex-flow: row nowrap;
    height: 30px;
    outline: none;
    max-width: 100%;
    max-height: 100%;
}

.number_chooser input {
    width: 60%;
}

.number_chooser button {
    height: 100%;
    width: 20%;
    flex-grow: 1;
    flex-shrink: 0;
    background-color: white;
    outline: none;
    border-color: black;
    border-width: 10px;
}

button svg {
    height: 60%;
    width: 60%;
}

.num_chooser_button_minus {
    border-top-left-radius: 15px;
    border-bottom-left-radius: 15px;
    order: 0;
    color: #b40000;
}

.num_chooser_input {
    -moz-appearance: textfield;
    text-align: center;
    height: 100%;
    width: 60%;
    flex-grow: 2;
    flex-shrink: 0;
    outline: none;
    order: 1;
}

.num_chooser_button_plus {
    border-top-right-radius: 15px;
    border-bottom-right-radius: 15px;
    order: 2;
    color: #009513;
}


.num_chooser_input:invalid {
    box-shadow: none;
    background-color: #e3e3e3;
    color: #6d6d6d;
}

.num_chooser_input::-webkit-inner-spin-button,
.num_chooser_input::-webkit-outer-spin-button,
.num_chooser_input:hover::-webkit-inner-spin-button,
.num_chooser_input:hover::-webkit-outer-spin-button {
    -moz-appearance: none;
    -webkit-appearance: none;
    appearance: none;
    margin: 0;
}
</style>