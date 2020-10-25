let f = (event, customName) => {
  let _ =
    Webapi.Dom.Element.dispatchEvent(
      Webapi.Dom.CustomEvent.makeWithOptions(
        customName,
        {
          "detail": {
            "identifier": ReactEvent.Pointer.pointerId(event),
          },
        },
      ),
      Webapi.Dom.Document.elementFromPoint(
        ReactEvent.Pointer.clientX(event),
        ReactEvent.Pointer.clientY(event),
        Webapi.Dom.document,
      ),
    );
  ();
};
