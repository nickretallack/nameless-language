const path = require('path')

module.exports = {
	entry: './src/Index.bs.js',
	mode: 'production',
	output: {
		path: path.join(__dirname, 'docs'),
		filename: 'index.js',
	},
}
