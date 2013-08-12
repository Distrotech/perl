/*    inline.h
 *
 *    Copyright (C) 2012 by Larry Wall and others
 *
 *    You may distribute under the terms of either the GNU General Public
 *    License or the Artistic License, as specified in the README file.
 *
 * This file is a home for static inline functions that cannot go in other
 * headers files, because they depend on proto.h (included after most other
 * headers) or struct definitions.
 *
 * Each section names the header file that the functions "belong" to.
 */

/* ------------------------------- av.h ------------------------------- */

PERL_STATIC_INLINE I32
S_av_top_index(pTHX_ AV *av)
{
    PERL_ARGS_ASSERT_AV_TOP_INDEX;
    assert(SvTYPE(av) == SVt_PVAV);

    return AvFILL(av);
}

/* ------------------------------- cv.h ------------------------------- */

PERL_STATIC_INLINE I32 *
S_CvDEPTHp(const CV * const sv)
{
    assert(SvTYPE(sv) == SVt_PVCV || SvTYPE(sv) == SVt_PVFM);
    return &((XPVCV*)SvANY(sv))->xcv_depth;
}

/*
 CvPROTO returns the prototype as stored, which is not necessarily what
 the interpreter should be using. Specifically, the interpreter assumes
 that spaces have been stripped, which has been the case if the prototype
 was added by toke.c, but is generally not the case if it was added elsewhere.
 Since we can't enforce the spacelessness at assignment time, this routine
 provides a temporary copy at parse time with spaces removed.
 I<orig> is the start of the original buffer, I<len> is the length of the
 prototype and will be updated when this returns.
 */

#ifdef PERL_CORE
PERL_STATIC_INLINE char *
S_strip_spaces(pTHX_ const char * orig, STRLEN * const len)
{
    SV * tmpsv;
    char * tmps;
    tmpsv = newSVpvn_flags(orig, *len, SVs_TEMP);
    tmps = SvPVX(tmpsv);
    while ((*len)--) {
	if (!isSPACE(*orig))
	    *tmps++ = *orig;
	orig++;
    }
    *tmps = '\0';
    *len = tmps - SvPVX(tmpsv);
		return SvPVX(tmpsv);
}
#endif

/* ----------------------------- regexp.h ----------------------------- */

PERL_STATIC_INLINE struct regexp *
S_ReANY(const REGEXP * const re)
{
    assert(isREGEXP(re));
    return re->sv_u.svu_rx;
}

/* ------------------------------- sv.h ------------------------------- */

PERL_STATIC_INLINE SV *
S_SvREFCNT_inc(SV *sv)
{
    if (LIKELY(sv != NULL))
	SvREFCNT(sv)++;
    return sv;
}
PERL_STATIC_INLINE SV *
S_SvREFCNT_inc_NN(SV *sv)
{
    SvREFCNT(sv)++;
    return sv;
}
PERL_STATIC_INLINE void
S_SvREFCNT_inc_void(SV *sv)
{
    if (LIKELY(sv != NULL))
	SvREFCNT(sv)++;
}
PERL_STATIC_INLINE void
S_SvREFCNT_dec(pTHX_ SV *sv)
{
    if (LIKELY(sv != NULL)) {
	U32 rc = SvREFCNT(sv);
	if (LIKELY(rc > 1))
	    SvREFCNT(sv) = rc - 1;
	else
	    Perl_sv_free2(aTHX_ sv, rc);
    }
}

PERL_STATIC_INLINE void
S_SvREFCNT_dec_NN(pTHX_ SV *sv)
{
    U32 rc = SvREFCNT(sv);
    if (LIKELY(rc > 1))
	SvREFCNT(sv) = rc - 1;
    else
	Perl_sv_free2(aTHX_ sv, rc);
}

PERL_STATIC_INLINE void
SvAMAGIC_on(SV *sv)
{
    assert(SvROK(sv));
    if (SvOBJECT(SvRV(sv))) HvAMAGIC_on(SvSTASH(SvRV(sv)));
}
PERL_STATIC_INLINE void
SvAMAGIC_off(SV *sv)
{
    if (SvROK(sv) && SvOBJECT(SvRV(sv)))
	HvAMAGIC_off(SvSTASH(SvRV(sv)));
}

PERL_STATIC_INLINE U32
S_SvPADTMP_on(SV *sv)
{
    assert(!(SvFLAGS(sv) & SVs_PADMY));
    return SvFLAGS(sv) |= SVs_PADTMP;
}
PERL_STATIC_INLINE U32
S_SvPADTMP_off(SV *sv)
{
    assert(!(SvFLAGS(sv) & SVs_PADMY));
    return SvFLAGS(sv) &= ~SVs_PADTMP;
}
PERL_STATIC_INLINE U32
S_SvPADSTALE_on(SV *sv)
{
    assert(SvFLAGS(sv) & SVs_PADMY);
    return SvFLAGS(sv) |= SVs_PADSTALE;
}
PERL_STATIC_INLINE U32
S_SvPADSTALE_off(SV *sv)
{
    assert(SvFLAGS(sv) & SVs_PADMY);
    return SvFLAGS(sv) &= ~SVs_PADSTALE;
}
#ifdef PERL_CORE
PERL_STATIC_INLINE STRLEN
S_sv_or_pv_pos_u2b(pTHX_ SV *sv, const char *pv, STRLEN pos, STRLEN *lenp)
{
    if (SvGAMAGIC(sv)) {
	U8 *hopped = utf8_hop((U8 *)pv, pos);
	if (lenp) *lenp = (STRLEN)(utf8_hop(hopped, *lenp) - hopped);
	return (STRLEN)(hopped - (U8 *)pv);
    }
    return sv_pos_u2b_flags(sv,pos,lenp,SV_CONST_RETURN);
}
#endif

/* ------------------------------- handy.h ------------------------------- */

/* saves machine code for a common noreturn idiom typically used in Newx*() */
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#endif
static void
S_croak_memory_wrap(void)
{
    Perl_croak_nocontext("%s",PL_memory_wrap);
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif

/* ------------------------------- utf8.h ------------------------------- */

/* These exist only to replace the macros they formerly were so that their use
 * can be deprecated */

PERL_STATIC_INLINE bool
S_isIDFIRST_lazy(pTHX_ const char* p)
{
    PERL_ARGS_ASSERT_ISIDFIRST_LAZY;

    return isIDFIRST_lazy_if(p,1);
}

PERL_STATIC_INLINE bool
S_isALNUM_lazy(pTHX_ const char* p)
{
    PERL_ARGS_ASSERT_ISALNUM_LAZY;

    return isALNUM_lazy_if(p,1);
}

/* ------------------------------- perl.h ----------------------------- */

/*
=for apidoc AiR|bool|is_safe_syscall|SV *pv|const char *what|const char *op_name

Test that the given C<pv> doesn't contain any internal NUL characters.
If it does, set C<errno> to ENOENT, optionally warn, and return FALSE.

Return TRUE if the name is safe.

Used by the IS_SAFE_SYSCALL() macro.

=cut
*/

PERL_STATIC_INLINE bool
S_is_safe_syscall(pTHX_ SV *pv, const char *what, const char *op_name) {
    /* While the Windows CE API provides only UCS-16 (or UTF-16) APIs
     * perl itself uses xce*() functions which accept 8-bit strings.
     */

    PERL_ARGS_ASSERT_IS_SAFE_SYSCALL;

    if (SvPOK(pv) && SvCUR(pv) >= 1) {
        char *p = SvPVX(pv);
        char *null_at;
        if (UNLIKELY((null_at = (char *)memchr(p, 0, SvCUR(pv)-1)) != NULL)) {
                SETERRNO(ENOENT, LIB_INVARG);
                if (ckWARN(WARN_SYSCALLS)) {
                    Perl_ck_warner(aTHX_ packWARN(WARN_SYSCALLS),
                                   "Invalid \\0 character in %s for %s: %s\\0%s",
                                   what, op_name, p, null_at+1);
                }
                return FALSE;
        }
    }

    return TRUE;
}

/*
 * Local variables:
 * c-indentation-style: bsd
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 *
 * ex: set ts=8 sts=4 sw=4 et:
 */
