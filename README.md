# [nfader~ ]
## A n-way linear pairwise cross-fader for Pure Data

Inlets:

- [1] takes a parameter between 0 and 1 to select the position between the
  second and the last inlet
- [2..n] take signals to interpolate between, pairwise

Outlet: spits out the interpolated signal.

The creation argument (defaults to 2) sets the number of signals to cross-fade
between. As you move through the 0-1 space, the object cross-fades linearly
between the two signals closest to the position of the parameter fed into the
first inlet.

For example, if you have three signals and you feed 0.25 to the first inlet,
then you are halfway between the second and the third inlets. If you feed 0.5
to the first inlet, you select purely the third inlet. If you feed 0.75 to the
first inlet, you are halfway between the third and the fourth inlet. It's
easier to hear it for yourself, though!

## deken

I'd love to push this to deken but I can only build for macOS at the moment.
If you have access to Windows and Linux and would like to help, please get
in touch!

## Your code sucks!

That's not unlikely. It's my very first Pure Data external and I did struggle
a little bit. Feel free to use github's issues and pull requests to suggest
improvements.

## How to make your computer go BOOP

I made this to support my series of [lazy tutorials](https://www.youtube.com/watch?v=AszBiqrvAaU&list=PL70paWUyPRgqhqxG0ShlROn1eSHlekhB_)
on YouTube. Have a look, like and subscribe if you like what you see!

I am also on [Patreon](https://www.patreon.com/morpheu5) if you want to help
me produce more videos, software, stuff... I'm more than happy to accept
your tokens of appreciation! (as long are they're made of gold! :)