                if(fl & CHFL_OWNER)
                {
                        *mbuf++ = 'q';
                        para[pargs++] = target_p->name;

                        if(fl & CHFL_ADMIN)
                        {
                                /* its possible the +y has filled up MAXMODEPARAMS, if so, start
                                 * a new buffer
                                 */
                                if(pargs >= MAXMODEPARAMS)
                                {
                                        *mbuf = '\0';
                                        sendto_channel_local(ALL_MEMBERS, chptr,
                                                        ":%s MODE %s %s %s %s %s %s",
                                                        fakesource_p->name, chptr->chname,
                                                        modebuf,
                                                        para[0], para[1], para[2], para[3]);
                                        mbuf = modebuf;
                                        *mbuf++ = '+';
                                        para[0] = para[1] = para[2] = para[3] = NULL;
                                        pargs = 0;
                                }

                                *mbuf++ = 'a';
                                para[pargs++] = target_p->name;
                        }
                        if(fl & CHFL_CHANOP)
                        {
                               /* its possible the +q has filled up MAXMODEPARAMS, if so, start
                                * a new buffer
                                */
                                if(pargs >= MAXMODEPARAMS)
                                {
                                        *mbuf = '\0';
                                        sendto_channel_local(ALL_MEMBERS, chptr,
                                                     ":%s MODE %s %s %s %s %s %s",
                                                     fakesource_p->name, chptr->chname,
                                                     modebuf,
                                                     para[0], para[1], para[2], para[3]);
                                        mbuf = modebuf;
                                        *mbuf++ = '+';
                                        para[0] = para[1] = para[2] = para[3] = NULL;
                                        pargs = 0;
                                }
                        
                                *mbuf++ = 'o';
                                para[pargs++] = target_p->name;
                        }
                        if(fl & CHFL_HALFOP)
                        {
                                /* its possible the +q has filled up MAXMODEPARAMS, if so, start
                                 * a new buffer
                                 */
                                if(pargs >= MAXMODEPARAMS)
                                {
                                        *mbuf = '\0';
                                        sendto_channel_local(ALL_MEMBERS, chptr,
                                                     ":%s MODE %s %s %s %s %s %s",
                                                     fakesource_p->name, chptr->chname,
                                                     modebuf,
                                                     para[0], para[1], para[2], para[3]);
                                        mbuf = modebuf;
                                        *mbuf++ = '+';
                                        para[0] = para[1] = para[2] = para[3] = NULL;
                                        pargs = 0;
                                }

                                *mbuf++ = 'h';
                                para[pargs++] = target_p->name;
                        }
                        if(fl & CHFL_VOICE)
                        {
                                /* its possible the +q has filled up MAXMODEPARAMS, if so, start
                                 * a new buffer
                                 */
                                if(pargs >= MAXMODEPARAMS)
                                {
                                        *mbuf = '\0';
                                        sendto_channel_local(ALL_MEMBERS, chptr,
                                                     ":%s MODE %s %s %s %s %s %s",
                                                     fakesource_p->name, chptr->chname,
                                                     modebuf,
                                                     para[0], para[1], para[2], para[3]);
                                mbuf = modebuf;
                                *mbuf++ = '+';
                                para[0] = para[1] = para[2] = para[3] = NULL;
                                pargs = 0;
                                }

                                *mbuf++ = 'v';
                                para[pargs++] = target_p->name;
                        }
                }
                else if(fl & CHFL_ADMIN)
		{
			*mbuf++ = 'a';
			para[pargs++] = target_p->name;

			if(fl & CHFL_CHANOP)
			{
				/* its possible the +a has filled up MAXMODEPARAMS, if so, start
				 * a new buffer
				 */
				if(pargs >= MAXMODEPARAMS)
				{
					*mbuf = '\0';
					sendto_channel_local(ALL_MEMBERS, chptr,
							     ":%s MODE %s %s %s %s %s %s",
							     fakesource_p->name, chptr->chname,
							     modebuf,
							     para[0], para[1], para[2], para[3]);
					mbuf = modebuf;
					*mbuf++ = '+';
					para[0] = para[1] = para[2] = para[3] = NULL;
					pargs = 0;
				}

				*mbuf++ = 'o';
				para[pargs++] = target_p->name;
			}
			if(fl & CHFL_HALFOP)
			{
				/* its possible the +a has filled up MAXMODEPARAMS, if so, start
				 * a new buffer
				 */
				if(pargs >= MAXMODEPARAMS)
				{
					*mbuf = '\0';
					sendto_channel_local(ALL_MEMBERS, chptr,
							     ":%s MODE %s %s %s %s %s %s",
							     fakesource_p->name, chptr->chname,
							     modebuf,
							     para[0], para[1], para[2], para[3]);
					mbuf = modebuf;
					*mbuf++ = '+';
					para[0] = para[1] = para[2] = para[3] = NULL;
					pargs = 0;
				}

				*mbuf++ = 'h';
				para[pargs++] = target_p->name;
			}
			if(fl & CHFL_VOICE)
			{
				/* its possible the +a has filled up MAXMODEPARAMS, if so, start
				 * a new buffer
				 */
				if(pargs >= MAXMODEPARAMS)
				{
					*mbuf = '\0';
					sendto_channel_local(ALL_MEMBERS, chptr,
							     ":%s MODE %s %s %s %s %s %s",
							     fakesource_p->name, chptr->chname,
							     modebuf,
							     para[0], para[1], para[2], para[3]);
					mbuf = modebuf;
					*mbuf++ = '+';
					para[0] = para[1] = para[2] = para[3] = NULL;
					pargs = 0;
				}

				*mbuf++ = 'v';
				para[pargs++] = target_p->name;
			}
		}
		else if(fl & CHFL_CHANOP)
		{
			*mbuf++ = 'o';
			para[pargs++] = target_p->name;

			if(fl & CHFL_HALFOP)
			{
				/* its possible the +o has filled up MAXMODEPARAMS, if so, start
				 * a new buffer
				 */
				if(pargs >= MAXMODEPARAMS)
				{
					*mbuf = '\0';
					sendto_channel_local(ALL_MEMBERS, chptr,
							     ":%s MODE %s %s %s %s %s %s",
							     fakesource_p->name, chptr->chname,
							     modebuf,
							     para[0], para[1], para[2], para[3]);
					mbuf = modebuf;
					*mbuf++ = '+';
					para[0] = para[1] = para[2] = para[3] = NULL;
					pargs = 0;
				}

				*mbuf++ = 'h';
				para[pargs++] = target_p->name;
			}
			if(fl & CHFL_VOICE)
			{
				/* its possible the +o has filled up MAXMODEPARAMS, if so, start
				 * a new buffer
				 */
				if(pargs >= MAXMODEPARAMS)
				{
					*mbuf = '\0';
					sendto_channel_local(ALL_MEMBERS, chptr,
							     ":%s MODE %s %s %s %s %s %s",
							     fakesource_p->name, chptr->chname,
							     modebuf,
							     para[0], para[1], para[2], para[3]);
					mbuf = modebuf;
					*mbuf++ = '+';
					para[0] = para[1] = para[2] = para[3] = NULL;
					pargs = 0;
				}

				*mbuf++ = 'v';
				para[pargs++] = target_p->name;
			}
		}
		else if(fl & CHFL_HALFOP)
		{
			*mbuf++ = 'h';
			para[pargs++] = target_p->name;

			if(fl & CHFL_VOICE)
			{
				/* its possible the +h has filled up MAXMODEPARAMS, if so, start
				 * a new buffer
				 */
				if(pargs >= MAXMODEPARAMS)
				{
					*mbuf = '\0';
					sendto_channel_local(ALL_MEMBERS, chptr,
							     ":%s MODE %s %s %s %s %s %s",
							     fakesource_p->name, chptr->chname,
							     modebuf,
							     para[0], para[1], para[2], para[3]);
					mbuf = modebuf;
					*mbuf++ = '+';
					para[0] = para[1] = para[2] = para[3] = NULL;
					pargs = 0;
				}

				*mbuf++ = 'v';
				para[pargs++] = target_p->name;
			}
		}
		else if(fl & CHFL_VOICE)
		{
			*mbuf++ = 'v';
			para[pargs++] = target_p->name;
		}
