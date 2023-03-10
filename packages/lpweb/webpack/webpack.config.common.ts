import HtmlWebpackPlugin from "html-webpack-plugin"
import path from "path"
import webpack from "webpack"

export const commonConfig: webpack.Configuration = {
  entry: `./src/index.tsx`,
  // https://webpack.js.org/plugins/split-chunks-plugin/
  optimization: {
    splitChunks: {
      chunks: `async`,
      minSize: 20000,
      minRemainingSize: 0,
      minChunks: 1,
      maxAsyncRequests: 30,
      maxInitialRequests: 30,
      enforceSizeThreshold: 50000,
      cacheGroups: {
        defaultVendors: {
          test: /[\\/]node_modules[\\/]/,
          priority: -10,
          reuseExistingChunk: true,
        },
        default: {
          minChunks: 2,
          priority: -20,
          reuseExistingChunk: true,
        },
      },
    },
  },
  module: {
    rules: [
      {
        test: /\.tsx?$/,
        use: `ts-loader`,
        exclude: /node_modules/,
      },
      {
        // This must match the wasm binary created in build-emscripten.sh
        test: /liblp\.js$/,
        loader: `exports-loader`,
        options: {
          type: `module`,
          exports: `liblp`,
        },
      },
      // wasm files should not be processed but just be emitted and we want
      // to have their public URL.
      {
        test: /liblp\.wasm$/,
        type: `javascript/auto`,
        loader: `file-loader`,
        // options: {
        // if you add this, wasm request path will be https://domain.com/publicpath/[hash].wasm
        //   publicPath: `static/`,
        // },
      },
    ],
  },
  resolve: {
    extensions: [ `.tsx`, `.ts`, `.js` ],
  },
  output: {
    filename: `bundle.js`,
    path: path.resolve(
      __dirname,
      `..`,
      `dist`
    ),
  },
  plugins: [
    new HtmlWebpackPlugin({
      template: path.join(
        __dirname,
        `..`,
        `public`,
        `index.html`
      ),
    }),
  ],
}
