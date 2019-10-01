void
chm_owner(struct Client *source_p, struct Channel *chptr,
          int alevel, int parc, int *parn,
          const char **parv, int *errors, int dir, char c, long mode_type)
{
        struct membership *mstptr;
        const char *ownernick;
        struct Client *targ_p;
        int override = 0;

        if(!ConfigChannel.use_owner)
        {
                if(*errors & SM_ERR_UNKNOWN)
                        return;
                *errors |= SM_ERR_UNKNOWN;
                sendto_one(source_p, form_str(ERR_UNKNOWNMODE), me.name, source_p->name, c);
                return;
        }

        if(alevel != CHFL_OWNER)
        {
                if(IsOverride(source_p))
                        override = 1;
                else
                {
                        if(!(*errors & SM_ERR_NOOPS))
                        sendto_one(source_p, ":%s 482 %s %s :You're not a channel owner", me.name, source_p->name, chptr->chname);
                        *errors |= SM_ERR_NOOPS;
                        return;
                }
        }

        if((dir == MODE_QUERY) || (parc <= *parn))
                return;

        ownernick = parv[(*parn)];
        (*parn)++;

        /* empty nick */
        if(EmptyString(ownernick))
        {
                sendto_one_numeric(source_p, ERR_NOSUCHNICK, form_str(ERR_NOSUCHNICK), "*");
                return;
        }

        if((targ_p = find_chasing(source_p, ownernick, NULL)) == NULL)
                return;

        mstptr = find_channel_membership(chptr, targ_p);

        if(mstptr == NULL)
        {
                if(!(*errors & SM_ERR_NOTONCHANNEL) && MyClient(source_p))
                        sendto_one_numeric(source_p, ERR_USERNOTINCHANNEL,
                                   form_str(ERR_USERNOTINCHANNEL), ownernick, chptr->chname);
                                   *errors |= SM_ERR_NOTONCHANNEL;
                return;
        }

        if(MyClient(source_p) && (++mode_limit > MAXMODEPARAMS))
                return;

        if(dir == MODE_ADD)
        {
                if(targ_p == source_p)
                {
                        no_override_deop = 1;
                        /* Don't reject modes from remote. It desyncs, and this is perfectly
                         * legitimate from a remote override oper.
                         if(!override)
                          return;
                         */
                }

                mode_changes[mode_count].letter = c;
                mode_changes[mode_count].dir = MODE_ADD;
                mode_changes[mode_count].caps = 0;
                mode_changes[mode_count].nocaps = 0;
                mode_changes[mode_count].mems = ALL_MEMBERS;
                mode_changes[mode_count].id = targ_p->id;
                mode_changes[mode_count].arg = targ_p->name;
                mode_changes[mode_count].override = override;
                mode_changes[mode_count++].client = targ_p;

                mstptr->flags |= CHFL_OWNER;
        }
        else
        {
                if(MyClient(source_p) && IsService(targ_p))
                {
                        sendto_one(source_p, form_str(ERR_ISCHANSERVICE),
                                   me.name, source_p->name, targ_p->name, chptr->chname);
                        return;
                }

                mode_changes[mode_count].letter = c;
                mode_changes[mode_count].dir = MODE_DEL;
                mode_changes[mode_count].caps = 0;
                mode_changes[mode_count].nocaps = 0;
                mode_changes[mode_count].mems = ALL_MEMBERS;
                mode_changes[mode_count].id = targ_p->id;
                mode_changes[mode_count].arg = targ_p->name;
                mode_changes[mode_count].override = override;
                mode_changes[mode_count++].client = targ_p;

                mstptr->flags &= ~CHFL_OWNER;
        }
}

void
chm_admin(struct Client *source_p, struct Channel *chptr,
       int alevel, int parc, int *parn,
       const char **parv, int *errors, int dir, char c, long mode_type)
{
	struct membership *mstptr;
	const char *adminnick;
	struct Client *targ_p;
	int override = 0;

	if(!ConfigChannel.use_admin)
	{
		if(*errors & SM_ERR_UNKNOWN)
			return;
		*errors |= SM_ERR_UNKNOWN;
		sendto_one(source_p, form_str(ERR_UNKNOWNMODE), me.name, source_p->name, c);
		return;
	}

	if(alevel != CHFL_ADMIN && alevel != CHFL_OWNER)
	{
		if(IsOverride(source_p))
			override = 1;
		else
		{

			if(!(*errors & SM_ERR_NOOPS))
				sendto_one(source_p, ":%s 482 %s %s :You're not a channel administrator", me.name, source_p->name, chptr->chname);
			*errors |= SM_ERR_NOOPS;
			return;
		}
	}

	if((dir == MODE_QUERY) || (parc <= *parn))
		return;

	adminnick = parv[(*parn)];
	(*parn)++;

	/* empty nick */
	if(EmptyString(adminnick))
	{
		sendto_one_numeric(source_p, ERR_NOSUCHNICK, form_str(ERR_NOSUCHNICK), "*");
		return;
	}

	if((targ_p = find_chasing(source_p, adminnick, NULL)) == NULL)
	{
		return;
	}

	mstptr = find_channel_membership(chptr, targ_p);

	if(mstptr == NULL)
	{
		if(!(*errors & SM_ERR_NOTONCHANNEL) && MyClient(source_p))
			sendto_one_numeric(source_p, ERR_USERNOTINCHANNEL,
					   form_str(ERR_USERNOTINCHANNEL), adminnick, chptr->chname);
		*errors |= SM_ERR_NOTONCHANNEL;
		return;
	}

	if(MyClient(source_p) && (++mode_limit > MAXMODEPARAMS))
		return;

	if(dir == MODE_ADD)
	{
		if(targ_p == source_p)
		{
			no_override_deop = 1;
			/* Don't reject modes from remote. It desyncs, and this is perfectly
			 * legitimate from a remote override oper.
			if(!override)
				return;
			*/
		}

		mode_changes[mode_count].letter = c;
		mode_changes[mode_count].dir = MODE_ADD;
		mode_changes[mode_count].caps = 0;
		mode_changes[mode_count].nocaps = 0;
		mode_changes[mode_count].mems = ALL_MEMBERS;
		mode_changes[mode_count].id = targ_p->id;
		mode_changes[mode_count].arg = targ_p->name;
		mode_changes[mode_count].override = override;
		mode_changes[mode_count++].client = targ_p;

		mstptr->flags |= CHFL_ADMIN;
	}
	else
	{
		if(MyClient(source_p) && IsService(targ_p))
		{
			sendto_one(source_p, form_str(ERR_ISCHANSERVICE),
				   me.name, source_p->name, targ_p->name, chptr->chname);
			return;
		}

		mode_changes[mode_count].letter = c;
		mode_changes[mode_count].dir = MODE_DEL;
		mode_changes[mode_count].caps = 0;
		mode_changes[mode_count].nocaps = 0;
		mode_changes[mode_count].mems = ALL_MEMBERS;
		mode_changes[mode_count].id = targ_p->id;
		mode_changes[mode_count].arg = targ_p->name;
		mode_changes[mode_count].override = override;
		mode_changes[mode_count++].client = targ_p;

		mstptr->flags &= ~CHFL_ADMIN;
	}
}

void
chm_op(struct Client *source_p, struct Channel *chptr,
       int alevel, int parc, int *parn,
       const char **parv, int *errors, int dir, char c, long mode_type)
{
	struct membership *mstptr;
	const char *opnick;
	struct Client *targ_p;
	int override = 0;

	if(alevel != CHFL_CHANOP && alevel != CHFL_ADMIN && alevel != CHFL_OWNER)
	{
		if(IsOverride(source_p))
			override = 1;
		else
		{

			if(!(*errors & SM_ERR_NOOPS))
				sendto_one(source_p, form_str(ERR_CHANOPRIVSNEEDED),
						me.name, source_p->name, chptr->chname);
			*errors |= SM_ERR_NOOPS;
			return;
		}
	}

	if((dir == MODE_QUERY) || (parc <= *parn))
		return;

	opnick = parv[(*parn)];
	(*parn)++;

	/* empty nick */
	if(EmptyString(opnick))
	{
		sendto_one_numeric(source_p, ERR_NOSUCHNICK, form_str(ERR_NOSUCHNICK), "*");
		return;
	}

	if((targ_p = find_chasing(source_p, opnick, NULL)) == NULL)
	{
		return;
	}

	mstptr = find_channel_membership(chptr, targ_p);

	if(mstptr == NULL)
	{
		if(!(*errors & SM_ERR_NOTONCHANNEL) && MyClient(source_p))
			sendto_one_numeric(source_p, ERR_USERNOTINCHANNEL,
					   form_str(ERR_USERNOTINCHANNEL), opnick, chptr->chname);
		*errors |= SM_ERR_NOTONCHANNEL;
		return;
	}

	if(MyClient(source_p) && (++mode_limit > MAXMODEPARAMS))
		return;

	if(dir == MODE_ADD)
	{
		if(targ_p == source_p)
		{
			no_override_deop = 1;
			/* Don't reject modes from remote. It desyncs, and this is perfectly
			 * legitimate from a remote override oper.
			if(!override)
				return;
			*/
		}

		mode_changes[mode_count].letter = c;
		mode_changes[mode_count].dir = MODE_ADD;
		mode_changes[mode_count].caps = 0;
		mode_changes[mode_count].nocaps = 0;
		mode_changes[mode_count].mems = ALL_MEMBERS;
		mode_changes[mode_count].id = targ_p->id;
		mode_changes[mode_count].arg = targ_p->name;
		mode_changes[mode_count].override = override;
		mode_changes[mode_count++].client = targ_p;

		mstptr->flags |= CHFL_CHANOP;
	}
	else
	{
		if(MyClient(source_p) && IsService(targ_p))
		{
			sendto_one(source_p, form_str(ERR_ISCHANSERVICE),
				   me.name, source_p->name, targ_p->name, chptr->chname);
			return;
		}

		mode_changes[mode_count].letter = c;
		mode_changes[mode_count].dir = MODE_DEL;
		mode_changes[mode_count].caps = 0;
		mode_changes[mode_count].nocaps = 0;
		mode_changes[mode_count].mems = ALL_MEMBERS;
		mode_changes[mode_count].id = targ_p->id;
		mode_changes[mode_count].arg = targ_p->name;
		mode_changes[mode_count].override = override;
		mode_changes[mode_count++].client = targ_p;

		mstptr->flags &= ~CHFL_CHANOP;
	}
}

void
chm_halfop(struct Client *source_p, struct Channel *chptr,
       int alevel, int parc, int *parn,
       const char **parv, int *errors, int dir, char c, long mode_type)
{
	struct membership *mstptr;
	const char *halfopnick;
	struct Client *targ_p;
	int override = 0;

	if(!ConfigChannel.use_halfop)
	{
		if(*errors & SM_ERR_UNKNOWN)
			return;
		*errors |= SM_ERR_UNKNOWN;
		sendto_one(source_p, form_str(ERR_UNKNOWNMODE), me.name, source_p->name, c);
		return;
	}

	if(alevel != CHFL_CHANOP && alevel != CHFL_ADMIN && alevel != CHFL_OWNER)
	{
		if(IsOverride(source_p))
			override = 1;
		else
		{

			if(!(*errors & SM_ERR_NOOPS))
				sendto_one(source_p, form_str(ERR_CHANOPRIVSNEEDED),
						me.name, source_p->name, chptr->chname);
			*errors |= SM_ERR_NOOPS;
			return;
		}
	}

	if((dir == MODE_QUERY) || (parc <= *parn))
		return;

	halfopnick = parv[(*parn)];
	(*parn)++;

	/* empty nick */
	if(EmptyString(halfopnick))
	{
		sendto_one_numeric(source_p, ERR_NOSUCHNICK, form_str(ERR_NOSUCHNICK), "*");
		return;
	}

	if((targ_p = find_chasing(source_p, halfopnick, NULL)) == NULL)
	{
		return;
	}

	mstptr = find_channel_membership(chptr, targ_p);

	if(mstptr == NULL)
	{
		if(!(*errors & SM_ERR_NOTONCHANNEL) && MyClient(source_p))
			sendto_one_numeric(source_p, ERR_USERNOTINCHANNEL,
					   form_str(ERR_USERNOTINCHANNEL), halfopnick, chptr->chname);
		*errors |= SM_ERR_NOTONCHANNEL;
		return;
	}

	if(MyClient(source_p) && (++mode_limit > MAXMODEPARAMS))
		return;

	if(dir == MODE_ADD)
	{
		if(targ_p == source_p)
		{
			no_override_deop = 1;
			/* Don't reject modes from remote. It desyncs, and this is perfectly
			 * legitimate from a remote override oper.
			if(!override)
				return;
			*/
		}

		mode_changes[mode_count].letter = c;
		mode_changes[mode_count].dir = MODE_ADD;
		mode_changes[mode_count].caps = 0;
		mode_changes[mode_count].nocaps = 0;
		mode_changes[mode_count].mems = ALL_MEMBERS;
		mode_changes[mode_count].id = targ_p->id;
		mode_changes[mode_count].arg = targ_p->name;
		mode_changes[mode_count].override = override;
		mode_changes[mode_count++].client = targ_p;

		mstptr->flags |= CHFL_HALFOP;
	}
	else
	{
		if(MyClient(source_p) && IsService(targ_p))
		{
			sendto_one(source_p, form_str(ERR_ISCHANSERVICE),
				   me.name, source_p->name, targ_p->name, chptr->chname);
			return;
		}

		mode_changes[mode_count].letter = c;
		mode_changes[mode_count].dir = MODE_DEL;
		mode_changes[mode_count].caps = 0;
		mode_changes[mode_count].nocaps = 0;
		mode_changes[mode_count].mems = ALL_MEMBERS;
		mode_changes[mode_count].id = targ_p->id;
		mode_changes[mode_count].arg = targ_p->name;
		mode_changes[mode_count].override = override;
		mode_changes[mode_count++].client = targ_p;

		mstptr->flags &= ~CHFL_HALFOP;
	}
}

