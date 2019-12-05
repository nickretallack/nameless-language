type t = {
  name: Translatable.t,
  description: Translatable.t,
  inputs: Belt.Map.String.t(Translatable.t),
  outputs: Belt.Map.String.t(Translatable.t),
};
