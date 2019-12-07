[@react.component]
let make =
    (
      ~sourcePosition: Point.t,
      ~sinkPosition: Point.t,
      ~color: string,
      ~nodeWidth: float,
      ~xPadding: float,
      ~segments: list(float)=[],
      ~sourceIndex: int,
      ~isSelected: bool=false,
      ~debugState: ConnectionDebug.t=NoDebugConnection,
      ~onClick=?,
    ) => {
  let nudge =
    5.0
    *. float_of_int(
         if (sourceIndex mod 2 == 0) {
           sourceIndex / 2;
         } else {
           - (sourceIndex + 1) / 2;
         },
       );

  let path =
    Printf.sprintf("M%f,%f ", sourcePosition.x, sourcePosition.y)
    ++ (
      if (Belt.List.length(segments) == 0) {
        CurveConnect.f(sourcePosition, sinkPosition, nudge);
      } else {
        let (parts, lastPosition) =
          Belt.List.reduce(
            segments, ([], sourcePosition), ((acc, lastPosition), segmentY) =>
            (
              [
                CurveConnect.f(
                  lastPosition,
                  {x: lastPosition.x -. xPadding, y: segmentY +. nudge},
                  nudge,
                )
                ++ Printf.sprintf(" h %f ", -. nodeWidth),
                ...acc,
              ],
              {
                x: lastPosition.x -. xPadding -. nodeWidth,
                y: segmentY +. nudge,
              },
            )
          );
        String.concat(
          "               ",
          Belt.List.reverse([
            CurveConnect.f(lastPosition, sinkPosition, nudge),
            ...parts,
          ]),
        );
      }
    );

  <path
    fill="transparent"
    stroke={
      switch (debugState) {
      | EvaluatingConnection => "blue"
      | ReturningConnection => "green"
      | NoDebugConnection => isSelected ? "red" : color
      }
    }
    strokeWidth="5"
    pointerEvents="visibleStroke"
    d=path
    ?onClick
  />;
};
