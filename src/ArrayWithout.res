let f: 'a. (array<'a>, 'a) => array<'a> = (array, needle) =>
  Belt.Array.keep(array, item => item != needle)
