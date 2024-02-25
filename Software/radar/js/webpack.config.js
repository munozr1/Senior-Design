const path = require('path');


module.exports = {
    entry: './radar.js',
    output: {
        filename: 'bundle.js',
        path: path.resolve("path-browserify"), // Output to your public folder
    }
};
