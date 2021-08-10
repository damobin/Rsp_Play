define set_args
	set args "" "../data/test.bin"
	b main
	r
end

define disall
	display inputMaxtrix
	display keyMaxtrix
	display bytepos
	display bitpos
	display *i
	display bitBuff

end
