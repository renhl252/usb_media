
MODULES = \
	mediaplayer 	\
	usb_detect 	

all clean :
	@ ErrModule="\n";  \
	for d in $(MODULES); do \
	  if $(MAKE) -C $$d $@; then \
	    echo '--------------- Module '$$d' Success! ---------------'; \
	  else \
	    echo '--------------- Module '$$d' Failed! ---------------'; \
	    ErrModule=$$ErrModule$$d'\n';	\
	  fi; \
	done; \
	echo '--------------- Make This Component Result: ---------------'; \
	if [ $$ErrModule = "\n" ]; then \
		echo 'All Modules Success!'; \
	else \
		echo 'One or more Module failed:'; \
		echo $$ErrModule;  \
		echo '---------------------------------------------------------';  \
	    	exit 1; \
	fi

.PHONY: test clean
