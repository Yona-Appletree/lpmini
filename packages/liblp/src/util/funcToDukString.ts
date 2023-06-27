/**
 * Convert a function to a string that can be used as a Duktape script, handling arrow functions at the top level,
 * but not doing more checking than that.
 *
 * @param func
 */
export function funcToDukString(
  func: Function
): string {
  const str = func.toString()
  if (!str.startsWith("function")) {
    throw new Error("Only standard functions are supported, no arrow functions: " + str)
  }
  return str
}
