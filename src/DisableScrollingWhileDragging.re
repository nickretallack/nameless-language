let f = element =>
  Webapi.Dom.Element.addEventListenerWithOptions(
    "touchstart",
    Webapi.Dom.Event.preventDefault,
    {"passive": false, "capture": false, "once": false},
    element,
  );
