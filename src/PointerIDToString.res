external pointerIDToInt: PointerID.t => int = "%identity"
let f = (pointerID: PointerID.t) => string_of_int(pointerIDToInt(pointerID))
