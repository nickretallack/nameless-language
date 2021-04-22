let f = (position: Point.t) =>
  ReactDOM.Style.make(
    ~left=FloatToPixels.f(position.x),
    ~top=FloatToPixels.f(position.y),
    (),
  );
