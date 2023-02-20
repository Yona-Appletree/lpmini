import type { Config } from "@jest/types"

const config: Config.InitialOptions = {
  verbose: true,
  rootDir: `..`,
  setupFiles: [ `<rootDir>/jest/setupTest.js` ],
  preset: `ts-jest`,
  testEnvironment: `jsdom`,
  "transform": {
    ".*/.+\\.[tj]sx?$": "ts-jest"
  },
}
export default config
