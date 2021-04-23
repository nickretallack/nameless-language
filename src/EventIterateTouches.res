let f = (event, callback) =>
  Array.iter(callback, JsToList.f(ReactEvent.Touch.changedTouches(event)))
