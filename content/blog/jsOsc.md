+++
title = "Online Syntesizer, Javascript with no dependencies"
description = "Simple use of browser api to generate sound"
date = 2024-12-24T15:00:00Z
draft = false

[taxonomies]
tags = ["javascript","synthesizer","Sound"]
[extra]
keywords = "Javascript,Synthesizer,Sound"
toc = true
series = "Sound"
+++

Some long time ago, feels like it was in another life, I was working on a media server and worked a lot with raw audio and video. And about the same time I heard something about Binaural beats and their potential healing properties. Yes, I like conspiracy theories! Some of theam are pretty good! =) I figured that it should be pretty easy to implement and experiment with it as much as I want. And it turned out that the easiest way to do it is in a browser. It is pretty easy with gstreamer in C++, but then making a CLI or UI will quickly become complex and convoluted. It is not a problem of course if you know what you are doing, but I knew it is something that will slow me down because I had no idea what I wanted to build.
This led me to javascript and after reading a source of a bunch of js libraries I found that native browser api already provides very nice high level API, similar to what I would get from gstreamer. So it was all set.
<!-- more -->

The goal was to implement sinusoidal audio wave signal with a different frequency into each ear. The idea is that if in the left ear you have 39.5Hz and in the right ear you get 40.5Hz, you will end up hearing a 1Hz difference on top of these two sounds. What makes this experiment different from just producing two waves and mixing them into one singal is that this mixing will happen in an absolutely different way. Normally sounds will just naturally combine together into a real analog sound wave and your ear will hear a combined wave like this:

{{ inline_html(src="/synth/waves_sum.html") }}

In this experiment however, "mixing" will happen inside your brain. The brain will just imagine that it hears this 1Hz frequency. This is really exciting! It is the same technique your brain uses when you watch a movie to make you believe that you are seeing very smooth movement, even though you are observing a stream of individual pictures changing each other 30 times per second. It performs a rather remarkable trick -  it imagines what would in-between pictures would be if you were looking at a very smooth movement. It does it so well that you would never think the motion wasn't smooth if you didn't know that. Another example of this trick is when you are looking at quickly moving light in the dark - try rotating a torch or a sparkler and you will see something like this:

{{ img(src="/synth/sparklers.jpg" class="b1" alt="Sparklers in darkness" caption="Sparklers" w=400 h=300) }}

Here is what I have got in the end(separate page is here: <a href="/synth/index.html" >synth</a>)

{{ inline_html(src="/synth/inline.html") }}

This was real fun! I could create as many oscillators as I want to and do all the experiments I want like:
1. Generating different sine waves and play them at the same time.
2. Generate different sine waves into different ears.
3. Generate the same sine wave from two oscillators at the same time.
4. Change frequencies in real time while playing.
5. Trying to generate a sound from multiple frequencies that will sound like a running engine.

With very little time I built something that worked, but it had these nasty clicks and pops when a new oscillator is added and even when the first one is started or stopped. While I really enjoyed playing with sound, this was a rather unpleasant effect. In order to get rid of it I implementated a small audio processor that is inserted in the audio pipeline. It works similar to a fade animation effect.

The code for it is opensource: <a href="https://github.com/justpresident/webOsc">github</a> There are lots of things that can be improved in the code, all contributions are welcome.
