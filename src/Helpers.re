open Definition;

let sortBy: 'a. (Belt.List.t('a), 'a => 'b) => Belt.List.t('a) =
  (list, func) =>
    Belt.List.map(
      Belt.List.sort(
        Belt.List.map(list, item => (func(item), item)),
        compare,
      ),
      snd,
    );

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
