let f = (elementRef: React.ref(Js.Nullable.t(Webapi.Dom.Element.t))) =>
  switch (Js.Nullable.toOption(elementRef.current)) {
  | None => ()
  | Some(element) => DisableScrollingWhileDragging.f(element)
  };

let undo = (elementRef: React.ref(Js.Nullable.t(Webapi.Dom.Element.t))) =>
  switch (Js.Nullable.toOption(elementRef.current)) {
  | None => ()
  | Some(element) => DisableScrollingWhileDragging.undo(element)
  };
