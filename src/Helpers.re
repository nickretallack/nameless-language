type point = {
  x: int,
  y: int,
};

let sortBy: 'a. (Belt.List.t('a), 'a => 'b) => Belt.List.t('a) =
  (list, func) =>
    Belt.List.map(
      Belt.List.sort(
        Belt.List.map(list, item => (func(item), item)),
        compare,
      ),
      snd,
    );

let rec findIndexExn: 'a. (Belt.List.t('a), 'a) => int =
  (list, needle) =>
    switch (list) {
    | [] => raise(Not_found)
    | [head, ...rest] => head == needle ? 0 : 1 + findIndexExn(rest, needle)
    };

let rec findByIndexExn: 'a. (Belt.List.t('a), 'a => bool) => int =
  (list, check) =>
    switch (list) {
    | [] => raise(Not_found)
    | [head, ...rest] => check(head) ? 0 : 1 + findByIndexExn(rest, check)
    };

Random.init(int_of_float(Js.Date.now()));

let randomHex = () => Printf.sprintf("%x", Random.int(16));
let randomId = () =>
  String.concat("", Belt.List.makeBy(32, _ => randomHex()));

let pixels = x => string_of_int(x) ++ "px";

let pointFromMouse = event => {
  x: ReactEvent.Mouse.clientX(event),
  y: ReactEvent.Mouse.clientY(event),
};

let positionStyle = (position: point) =>
  ReactDOMRe.Style.make(
    ~left=pixels(position.x),
    ~top=pixels(position.y),
    (),
  );

external convertToList: Js.t('a) => array('b) = "%identity";

let iterateTouches = (event, callback) =>
  Array.iter(
    callback,
    convertToList(ReactEvent.Touch.changedTouches(event)),
  );

let getEventValue = event => ReactEvent.Form.target(event)##value;

let renderMap = (fn, map) =>
  ReasonReact.array(Array.map(fn, Belt.Map.toArray(map)));
let renderStringMap = (fn, map) =>
  ReasonReact.array(Array.map(fn, Belt.Map.String.toArray(map)));
let renderList = (fn, list) =>
  ReasonReact.array(Array.of_list(List.mapi(fn, list)));
let renderArray = (fn, array) => ReasonReact.array(Array.map(fn, array));
