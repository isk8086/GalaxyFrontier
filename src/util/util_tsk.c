
#include <stdio.h>

#include "util_tsk.h"

static void TSKinitMember(TSK *tp);

TSK TskIndex[TSK_GROUP_MAX][2];
TSK TskBuf[TSK_MAX];
int TskEntCnt;

static TSK *TskEmptyptr;

void TSKinit(void)
{
	int	i;

	for(i = 0; i < TSK_MAX - 1; i++){
		TskBuf[i].emptr = &(TskBuf[i+1]);
	}

	TskEmptyptr = TskBuf;
	TskBuf[TSK_MAX-1].emptr = NULL;

	for(i = 0; i < TSK_GROUP_MAX; i++){
		TskIndex[i][TSK_TOP].prev_tsk = NULL;
		TskIndex[i][TSK_TOP].next_tsk = &(TskIndex[i][TSK_BOTTOM]);
		TskIndex[i][TSK_BOTTOM].prev_tsk = &(TskIndex[i][TSK_TOP]);
		TskIndex[i][TSK_BOTTOM].next_tsk = NULL;
	}
}

TSK *TSKsetTop(FUNC exec_adr, int group)
{
	TSK *tp;

	if(TskEmptyptr){
		tp = TskEmptyptr;
		TskEmptyptr = tp->emptr;
		TSKinitMember(tp);
		tp->prog_adr = exec_adr;
		tp->group_no = group;
		tp->prev_tsk = &(TskIndex[group][TSK_TOP]);
		tp->next_tsk = TskIndex[group][TSK_TOP].next_tsk;
		tp->prev_tsk->next_tsk = tp;
		tp->next_tsk->prev_tsk = tp;
		return tp;
	}

	return (TSK *)NULL;
}

TSK *TSKsetBtm(FUNC exec_adr, int group)
{
	TSK	*tp;

	if(TskEmptyptr){
		tp = TskEmptyptr;
		TskEmptyptr = tp->emptr;
		TSKinitMember(tp);
		tp->prog_adr = exec_adr;
		tp->group_no = group;
		tp->prev_tsk = TskIndex[group][TSK_BOTTOM].prev_tsk;
		tp->next_tsk = &(TskIndex[group][TSK_BOTTOM]);
		tp->prev_tsk->next_tsk = tp;
		tp->next_tsk->prev_tsk = tp;
		return tp;
	}

	return (TSK *)NULL;
}

TSK *TSKsetTopName(FUNC exec_adr, int group, unsigned int name)
{
	TSK *tp;

	if(TskEmptyptr){
		tp = TskEmptyptr;
		TskEmptyptr = tp->emptr;
		TSKinitMember(tp);
		tp->prog_adr = exec_adr;
		tp->group_no = group;
		tp->name = name;
		tp->prev_tsk = &(TskIndex[group][TSK_TOP]);
		tp->next_tsk = TskIndex[group][TSK_TOP].next_tsk;
		tp->prev_tsk->next_tsk = tp;
		tp->next_tsk->prev_tsk = tp;
		return tp;
	}

	return (TSK *)NULL;
}

TSK *TSKsetBtmName(FUNC exec_adr, int group, unsigned int name)
{
	TSK	*tp;

	if(TskEmptyptr){
		tp = TskEmptyptr;
		TskEmptyptr = tp->emptr;
		TSKinitMember(tp);
		tp->prog_adr = exec_adr;
		tp->group_no = group;
		tp->name = name;
		tp->prev_tsk = TskIndex[group][TSK_BOTTOM].prev_tsk;
		tp->next_tsk = &(TskIndex[group][TSK_BOTTOM]);
		tp->prev_tsk->next_tsk = tp;
		tp->next_tsk->prev_tsk = tp;
		return tp;
	}

	return (TSK *)NULL;
}

void TSKexec(void)
{
	TSK *tp;
	int i;

	TskEntCnt = 0;

	for(i = 0; i < TSK_GROUP_MAX; i++){
		for(tp = TskIndex[i][TSK_TOP].next_tsk; tp != &(TskIndex[i][TSK_BOTTOM]); tp = tp->next_tsk){
			++TskEntCnt;
			if(tp->tskid & TSKID_PAUSE){
				continue;
			}
			TSKexecDirect(tp);
		}
	}
}

void TSKexecDirect(TSK *tp)
{
	if(tp == NULL) return;
	if(tp->prog_adr) tp->prog_adr(tp);
}

void TSKdraw(void)
{
	TSK *tp;
	int i;

	for(i = 0; i < TSK_GROUP_MAX; i++){
		for(tp = TskIndex[i][TSK_TOP].next_tsk; tp != &(TskIndex[i][TSK_BOTTOM]); tp = tp->next_tsk){
			if(tp->draw_adr) tp->draw_adr(tp);
		}
	}
}

void TSKintExec(TSK *tp)
{
	if(tp->intr_adr) tp->intr_adr(tp);
}

void TSKclear(TSK *tp)
{
	TSK *prev;
	TSK *next;

	if(tp->exit_adr) tp->exit_adr(tp);

	prev = tp->next_tsk->prev_tsk = tp->prev_tsk;
	next = tp->prev_tsk->next_tsk = tp->next_tsk;

	tp->tskid = TSKID_NONE;
	tp->emptr = TskEmptyptr;
	TskEmptyptr = tp;

	tp->prev_tsk = prev;
	tp->next_tsk = next;
}

void TSKclearAll(TSK *tp)
{
	TSK *etp;
	TSK *ntp;
	int i;

	for(i = 0; i < TSK_GROUP_MAX; i++){
		for(etp = TskIndex[i][TSK_TOP].next_tsk; etp != &(TskIndex[i][TSK_BOTTOM]); etp = ntp){
			ntp = etp->next_tsk;
			if(etp != tp) TSKclear(etp);
		}
	}
}

void TSKclearGroup(TSK *tp, int group)
{
	TSK *etp;
	TSK *ntp;

	for(etp = TskIndex[group][TSK_TOP].next_tsk; etp != &(TskIndex[group][TSK_BOTTOM]); etp = ntp){
		ntp = etp->next_tsk;
		if(etp != tp) TSKclear(etp);
	}
}

void TSKclearName(TSK *tp, unsigned int name)
{
	TSK *etp;
	TSK *ntp;
	int i;

	for(i = 0; i < TSK_GROUP_MAX; i++){
		for(etp = TskIndex[i][TSK_TOP].next_tsk; etp != &(TskIndex[i][TSK_BOTTOM]); etp = ntp){
			ntp = etp->next_tsk;
			if(etp != tp && etp->name == name) TSKclear(etp);
		}
	}
}

void TSKclearGroupName(TSK *tp, int group, unsigned int name)
{
	TSK *etp;
	TSK *ntp;

	for(etp = TskIndex[group][TSK_TOP].next_tsk; etp != &(TskIndex[group][TSK_BOTTOM]); etp = ntp){
		ntp = etp->next_tsk;
		if(etp != tp && etp->name == name) TSKclear(etp);
	}
}

int TSKgetGroupCnt(int group)
{
	TSK *etp;
	TSK *ntp;
	int cnt = 0;

	for(etp = TskIndex[group][TSK_TOP].next_tsk; etp != &(TskIndex[group][TSK_BOTTOM]); etp = ntp){
		ntp = etp->next_tsk;
		cnt++;
	}

	return cnt;
}

int TSKgetGroupNameCnt(int group, unsigned int name)
{
	TSK *etp;
	TSK *ntp;
	int cnt = 0;

	for(etp = TskIndex[group][TSK_TOP].next_tsk; etp != &(TskIndex[group][TSK_BOTTOM]); etp = ntp){
		ntp = etp->next_tsk;
		if(etp->name == name) cnt++;
	}

	return cnt;
}

TSK *TSKgetName(unsigned int name)
{
	TSK *etp;
	TSK *ntp;
	int i;

	for(i = 0; i < TSK_GROUP_MAX; i++){
		for(etp = TskIndex[i][TSK_TOP].next_tsk; etp != &(TskIndex[i][TSK_BOTTOM]); etp = ntp){
			ntp = etp->next_tsk;
			if(etp->name == name) return etp;
		}
	}

	return NULL;
}

TSK *TSKgetGroupName(int group, unsigned int name)
{
	TSK *etp;
	TSK *ntp;

	for(etp = TskIndex[group][TSK_TOP].next_tsk; etp != &(TskIndex[group][TSK_BOTTOM]); etp = ntp){
		ntp = etp->next_tsk;
		if(etp->name == name) return etp;
	}

	return NULL;
}

void TSKinitMember(TSK *tp)
{
	int i;

	tp->tskid = TSKID_EXIST;

	tp->tsk_parent = NULL;
	tp->tsk_child = NULL;
	tp->tsk_collision = NULL;
	tp->tsk_target = NULL;
	tp->exit_adr = NULL;
	tp->draw_adr = NULL;
	tp->intr_adr = NULL;
	tp->name = 0;

	tp->id = 0;
	tp->serial = 0;
	tp->step = 0;
	tp->next = 0;
	tp->mode = 0;
	tp->wait = 0;
	tp->cnt = 0;
	tp->num = 0;
	tp->pat = 0;
	tp->pat_wait = 0;
	tp->tmp = 0;
	tp->i = 0;
	tp->j = 0;
	tp->k = 0;
	tp->frame = 0.0f;
	tp->ftmp = 0.0f;
	tp->str_tmp = NULL;

	for(i = 0; i < XY; i++){
		tp->pos[i] = 0.0f;
		tp->ofs[i] = 0.0f;
		tp->vec[i] = 0.0f;
		tp->vel[i] = 0.0f;
		tp->acc[i] = 0.0f;
		tp->cen[i] = 0.0f;
		tp->trg[i] = 0.0f;
		tp->rad[i] = 0.0f;
		tp->rot[i] = 0.0f;
		tp->ang[i] = 0.0f;
	}

	tp->energy = 0;
	tp->dest_rate = 1.0f;
	tp->collision = NULL;
	tp->collision_num = 0;

	tp->work = NULL;
}
