let f: 'a. (array<'a>, 'a, int) => array<'a> = (array, needle, newIndex) => {
  let index = ArrayFindIndexExn.f(array, needle)

  // extract
  let left = Belt.Array.slice(array, ~offset=0, ~len=index)
  let right = Belt.Array.sliceToEnd(array, index + 1)
  let arrayWithoutNeedle = Belt.Array.concat(left, right)

  // insert
  let left = Belt.Array.slice(arrayWithoutNeedle, ~offset=0, ~len=newIndex)
  let right = Belt.Array.sliceToEnd(arrayWithoutNeedle, newIndex)
  Belt.Array.concatMany([left, [needle], right])
}
