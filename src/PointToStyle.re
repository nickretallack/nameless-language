let f = (position: Point.t) =>
  ReactDOMRe.Style.make(
    ~left=FloatToPixels.f(position.x),
    ~top=FloatToPixels.f(position.y),
    (),
  );
