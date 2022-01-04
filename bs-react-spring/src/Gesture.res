/*
 const bind = useGesture(({ down, delta, velocity }) => {
   velocity = clamp(velocity, 1, 8)
   set({ xy: down ? delta : [0, 0], config: { mass: velocity, tension: 500 * velocity, friction: 50 } })
 })

 */

type props = {
  /* source event */
  event: ReactEvent.Mouse.t,
  /* dom node */
  target: {.},
  /* time tag */
  time: float,
  /* click coordinates (vec2) */
  initial: (float, float),
  /* page coordinates (vec2) */
  xy: (float, float),
  /* previous page coordinates (vec2) */
  previous: (float, float),
  /* delta offset (xy - initial) (vec2) */
  delta: (float, float),
  /* direction normal (vec2) */
  direction: (float, float),
  cancel: (. int) => unit,
  /* delta with book-keeping (vec2) */
  local: (float, float),
  /* drag momentuum / speed */
  velocity: float,
  /* delta distance */
  distance: float,
  /* mouse / touch down */
  down: bool,
  /* marks first event (mouse / touch down) */
  first: bool,
  /* arguments optionally passed to bind(a,b,c,d,..) */
  args: {.},
  /* arguments optionally returned by onActions eventHandler */
  temp: {.},
}
@module("react-with-gesture")
external _externalUseGesture: (. props => 'a, 'a) => Spring.Div.spreadProps = "useGesture"

let useGesture = _externalUseGesture
