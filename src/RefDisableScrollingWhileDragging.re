let f =
  ReactDOMRe.Ref.callbackDomRef(nullableElement =>
    switch (Js.Nullable.toOption(nullableElement)) {
    | None => ()
    | Some(element) => DisableScrollingWhileDragging.f(element)
    }
  );
