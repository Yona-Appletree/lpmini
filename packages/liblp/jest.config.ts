import type { Config } from "@jest/types"

const config: Config.InitialOptions = {
  verbose: true,
  rootDir: `.`,
  preset: `ts-jest`,
  testEnvironment: `node`,
  
  "transform": {
    ".*/.+\\.[tj]sx?$": "ts-jest"
  },

  // Thanks https://stackoverflow.com/a/54453691/966104
  "moduleNameMapper": {
    "src/(.*)": "<rootDir>/src/$1"
  }
}
export default config
