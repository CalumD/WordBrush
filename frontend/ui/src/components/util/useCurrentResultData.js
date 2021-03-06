import {reactive, readonly} from 'vue';
import axios from "axios";
import {BASE_URL} from '@/main';

const currentResultData = reactive({
    resultID: '',
    outputType: '',
    images: {},
    error: {}
});
const delay = ms => new Promise(res => setTimeout(res, ms));
const MAX_RETRIES = 5;

function fetchResultSet(resultSetID) {
    return new Promise((resolve) => {
        axios
            .get(
                `http://${BASE_URL}/api/v1/results/${resultSetID}`,
                {
                    responseType: "json",
                    timeout: 5000
                }
            )
            .then((response) => {
                if (response.status !== 202) {
                    currentResultData.resultID = resultSetID;
                    currentResultData.outputType = response.data.outputType;
                    currentResultData.images = response.data.words;
                    return resolve(true);
                }
                return resolve(false);
            })
            .catch((err) => {
                console.log('Error retrieving result set: ', err.response);
                reset();
                if (err.response.status >= 400) {
                    currentResultData.error = err.response.data.valueOf();
                    return resolve(true);
                }
                return resolve(false);
            });
    });
}

function reset() {
    currentResultData.resultID = '';
    currentResultData.outputType = '';
    currentResultData.images = {};
    currentResultData.error = {};
}

export function useCurrentResultData() {

    function getCurrentResult() {
        return readonly(currentResultData);
    }

    async function fetchNewResultSet(resultSetID) {
        let success = await fetchResultSet(resultSetID);
        let retries = MAX_RETRIES;

        while (!success && retries-- > 0) {
            await delay(2000);
            success = await fetchResultSet(resultSetID);
        }
        if (!success) {
            console.error('Couldn\'t get a response in time for a resultSet (' + resultSetID + '), giving up.');
            reset();
        }
    }

    function setAxiosError(err) {
        console.log("Axios request failed.", err);
        if (err.response.status >= 400) {
            currentResultData.error = err.response.data.valueOf();
        }
    }

    function acknowledgeError() {
        reset();
    }

    return {
        getCurrentResult,
        fetchNewResultSet,
        setAxiosError,
        acknowledgeError
    };
}