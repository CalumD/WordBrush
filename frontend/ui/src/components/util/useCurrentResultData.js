import {reactive, readonly} from 'vue';

const currentResultData = reactive({
    resultID: '',
    images: []
});

export function useCurrentResultData() {

    if (currentResultData.resultID === '') {
        search();
    }

    function getCurrentResult() {
        return readonly(currentResultData);
    }

    function search(url, opts) {
        console.log("Setting 1");
        currentResultData.resultID = '458ddcfd-7212-470e-9036-bbfb40a35f88';
        currentResultData.images = [
            {imageID: '0.svg', imageText: '6785'},
            {imageID: '1.svg', imageText: '=1abcdef!'},
            {imageID: '2.svg', imageText: 'The'},
            {imageID: '3.svg', imageText: 'quick'},
            {imageID: '4.svg', imageText: 'brown'},
            {imageID: '5.svg', imageText: 'fox'},
            {imageID: '6.svg', imageText: 'jumps'},
            {imageID: '7.svg', imageText: 'over'},
            {imageID: '8.svg', imageText: 'the'},
            {imageID: '9.svg', imageText: 'lazy'},
            {imageID: '10.svg', imageText: 'dog'}
        ]
    }

    function search2Tho(url, opts) {
        console.log("Setting 2");
        currentResultData.resultID = 'd3aa88e2-c754-41e0-8ba6-4198a34aa0a2';
        currentResultData.images = [
            {imageID: '80.svg', imageText: 'AARC'},
            {imageID: '81.svg', imageText: 'aardvark!'},
            {imageID: '82.svg', imageText: 'aardvarks'},
            {imageID: '83.svg', imageText: 'aardwolf'},
            {imageID: '84.svg', imageText: 'aardwolves'},
            {imageID: '85.svg', imageText: 'Aaren'},
            {imageID: '86.svg', imageText: 'Aargau'},
            {imageID: '87.svg', imageText: 'aargh'},
            {imageID: '88.svg', imageText: 'Aarhus'},
            {imageID: '89.svg', imageText: 'Aarika'},
            {imageID: '90.svg', imageText: 'Aaron'}
        ]
    }

    return {
        getCurrentResult,
        search,
        search2Tho
    };
}