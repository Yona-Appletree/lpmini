import { describe, expect, it } from "@jest/globals";
import { funcToDukString } from "src/util/funcToDukString";

describe(`funcToDukString`, () => {
  it(
    `handles standard functions`,
    () => {
      expect(
        // @ts-ignore
        funcToDukString(function (a, b) {
          return a + b
        })
      ).toEqual(`function (a, b) {
            return a + b;
        }`);
    }
  );

  it(
    `throws for arrow functions`,
    () => {
      expect(
        // @ts-ignore
        () => funcToDukString((a, b) => a + b)
      ).toThrowError()
    }
  );
});
